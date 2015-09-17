using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;

namespace ServiceAreaServer
{
	class Program
	{
		// 数据库服务器情报
		static ServerInfo _dbServerInfo;

		public static ServerInfo DbServerInfo
		{
			get { return _dbServerInfo; }
			set { _dbServerInfo = value; }
		}

		// 端口号:1981 用于ServiceAreaClient向ServiceAreaServer(中继服务端)发送采集数据report
		// 端口号:1982 用于UpdaterServer向UpdaterClient传送更新文件
		// 端口号:1983 用于ServiceAreaClient监听接收消息
		static int _port = 1981;

        public static int Port
        {
            get { return Program._port; }
            set { Program._port = value; }
        }

		// 标记与数据库的连接状态
		private static bool _dataBaseConnected = false;

		public static bool DataBaseConnected
		{
			get { return Program._dataBaseConnected; }
			set { Program._dataBaseConnected = value; }
		}

		// 当某次写入数据库失败时, 以后用来尝试写入数据库, 检查连接是否恢复的数据内容
		// 一般是断网时第一条被缓存的数据
		private static string _attemptDataStr = string.Empty;

		public static string AttemptDataStr
		{
			get { return Program._attemptDataStr; }
			set { Program._attemptDataStr = value; }
		}

		static Queue<string> bufferQueue = new Queue<string>();

		// 用于定期想磁盘文件(断网时缓冲数据用)写入的timer
		static System.Timers.Timer bufferTimer;

		static void Main(string[] args)
		{
			// 读设定文件
			LoadIniFile();

			IPEndPoint ipep = new IPEndPoint(IPAddress.Any, Port);				// 本机预使用的IP和端口
			Socket sSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

			sSocket.Bind(ipep);													// 绑定
			sSocket.Listen(10);													// 监听

			// 第一层的主循环, 循环接收来自各个客户端的数据
			while (true)
			{
				try
				{
					Console.WriteLine("ServiceArea Relay Server(20150914)");
					Console.WriteLine("Waiting for a client...");
					Socket cSocket = sSocket.Accept();							// 当有可用的客户端连接尝试时执行，并返回一个新的socket,用于与客户端之间的通信
					IPEndPoint clientip = (IPEndPoint)cSocket.RemoteEndPoint;
					Console.WriteLine("Connect with client:" + clientip.Address + " at port:" + clientip.Port);

					string recvStr = "";
					byte[] recvBytes = new byte[1024];
					int bytes;
					bytes = cSocket.Receive(recvBytes, recvBytes.Length, 0);	// 从客户端接受消息
					recvStr += Encoding.ASCII.GetString(recvBytes, 0, bytes);
					Console.WriteLine("Server get message:{0}", recvStr);		// 把客户端传来的信息显示出来

					// 处理客户端发来的消息
					ClientMsgProcess(recvStr);
					cSocket.Close();
				}
				catch (Exception ex)
				{
					Console.WriteLine(ex.ToString());
				}
				finally
				{
				}
			}
			//sSocket.Close();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="clientMsg"></param>
		/// <param name="isReSend">标识是否是重发</param>
		/// <returns></returns>
		private static bool ClientMsgProcess(string clientMsg)
		{
			if (false == DataBaseConnected)
			{																	// 如果处于DB未连接状态
				if (string.Empty == AttemptDataStr)
				{																// 检查当前有无尝试写入字串
					string str = CheckLocalBuffer();							// 没有的话从缓存文件中load第一条
					if (string.Empty != str)
					{
						AttemptDataStr = str;
					}
					else
					{															// 缓存文件也没有就用新收到的数据
						AttemptDataStr = clientMsg;
					}
				}
				// 用尝试字串去尝试写入DB
				if (WriteToDB(AttemptDataStr))
				{
					// 如果成功, 说明与DB的连接成功或者断网后恢复了, load所有之前缓存的数据, 准备送出去
					// 先送缓存数据(去掉第一条已送出的)
					List<string> bufferList = LoadLocalBufferData();
					List<string> failList = new List<string>();					// 送失败的列表
					foreach (string dataStr in bufferList)
					{
						if (dataStr.Equals(AttemptDataStr))
						{
							continue;
						}
						if (!WriteToDB(dataStr))
						{
							failList.Add(dataStr);
						}
					}
					// 再发缓存Queue内未写入文件的数据
					lock (bufferQueue)
					{
						int count = bufferQueue.Count;
						for (int i = 0; i < count; i++)
						{
							string dataStr = bufferQueue.Dequeue();
							if (!WriteToDB(dataStr))
							{
								failList.Add(dataStr);
							}
						}
					}
					// 最后发新收到的数据, 如果新收到的数据以作为尝试字串发出去了就不要再发了
					if ((AttemptDataStr != clientMsg)
						&& !WriteToDB(clientMsg)	)
					{
						failList.Add(clientMsg);
					}
					AttemptDataStr = string.Empty;
					// 最后迁入连接状态
					if (0 == failList.Count)
					{
						// 都成功送出去了说明DB连接恢复了
						DataBaseConnected = true;
						// 清除缓存文件
						DeleteLocalFile();
						return true;
					}
					else
					{
						// 有失败的已然还是未连接状态, 将失败的写入本地缓存文件
						foreach (string dataStr in failList)
						{
							SaveToLocalFile(dataStr);
						}
						failList.Clear();
						return false;
					}
				}
				else
				{
					// 如果失败
					// 把新收到的数据压入Queue, 定期写入磁盘文件
					// 如果Queue是空的要起timer(用于定期向磁盘文件写入)
					lock (bufferQueue)
					{
						bufferQueue.Enqueue(clientMsg);
						if (1 == bufferQueue.Count)
						{
							// 如果是第一条, 起timer每分钟定时写入磁盘文件
							bufferTimer = new System.Timers.Timer(1000 * 60);
							bufferTimer.Elapsed += bufferTimer_Elapsed;
							bufferTimer.Start();
						}
					}
				}
				return false;
			}
			else
			{																	// 处于DB连接状态
				// 执行MySQL命令
				if (WriteToDB(clientMsg))
				{
					// 数据库保存成功
					Console.WriteLine("☆☆☆ Save To DB Success! ☆☆☆");
					return true;
				}
				else
				{
					// 数据库保存失败
					DataBaseConnected = false;
					bufferQueue.Enqueue(clientMsg);
					Console.WriteLine("※※※ Save To DB Fail! ※※※");
					return false;
				}
			}
		}

		static void bufferTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			lock (bufferQueue)
			{
				int count = bufferQueue.Count;
				for (int i = 0; i < count; i++)
				{
					string dataStr = bufferQueue.Dequeue();
					SaveToLocalFile(dataStr);
				}
			}
		}

		static bool WriteToDB(string cmdStr)
		{
			try
			{
				// 执行MySQL命令
				DBConnectMySQL mysql_object = new DBConnectMySQL(DbServerInfo);
				mysql_object.ExecuteMySqlCommand(cmdStr);
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.ToString());
				return false;
			}
			return true;
		}

		private static void LoadIniFile()
		{
			string host = IniFile.IniReadValue("DB_SERVER_INFO", "HOST");
			string portStr = IniFile.IniReadValue("DB_SERVER_INFO", "PORT");
			int port;
			if (!int.TryParse(portStr, out port))
			{
				return;
			}
			string db_name = IniFile.IniReadValue("DB_SERVER_INFO", "DB_NAME");
			string user_id = IniFile.IniReadValue("DB_SERVER_INFO", "USR_NAME");
			string pass_word = IniFile.IniReadValue("DB_SERVER_INFO", "PASSWORD");

			DbServerInfo = new ServerInfo(host, port, db_name, user_id, pass_word);

            portStr = IniFile.IniReadValue("LOCAL_INFO", "PORT");
            if (!int.TryParse(portStr, out port))
            {
                return;
            }
            Port = port;
		}

		// 断网时, 临时缓存数据用的本地文件
		const string _localBufFileName = "DisconnectBuffer.txt";

		public static string LocalBufFileName
		{
			get { return _localBufFileName; }
		}

		// 读写本地缓存文件时, 用的lock对象
		protected static object bufferLock = new object();

		static void SaveToLocalFile(string cmdStr)
		{
			lock (bufferLock)
			{
				try
				{
					StreamWriter sw = new StreamWriter(LocalBufFileName, true);
					sw.WriteLine(cmdStr);
					sw.Close();
				}
				catch (Exception ex)
				{
					Console.WriteLine(ex.ToString());
				}
			}
		}

		static void DeleteLocalFile()
		{
			lock (bufferLock)
			{
				try
				{
					File.Delete(LocalBufFileName);
				}
				catch (Exception ex)
				{
					Console.WriteLine(ex.ToString());
				}
			}
		}

		static string CheckLocalBuffer()
		{
			if (File.Exists(LocalBufFileName))
			{
				lock (bufferLock)
				{
					try
					{
						StreamReader sr = new StreamReader(LocalBufFileName);
						string rdLine = sr.ReadLine();
						sr.Close();
						if (null != rdLine
							&& string.Empty != rdLine.Trim())
						{
							return rdLine;
						}
					}
					catch (Exception ex)
					{
						Console.WriteLine(ex.ToString());
					}
				}
			}
			return string.Empty;
		}

		static List<string> LoadLocalBufferData()
		{
			List<string> rtDataList = new List<string>();
			lock (bufferLock)
			{
				try
				{
					StreamReader sr = new StreamReader(LocalBufFileName);
					do
					{
						string rdLine = sr.ReadLine();
						if (null == rdLine)
						{
							break;
						}
						if (string.Empty != rdLine.Trim())
						{
							rtDataList.Add(rdLine.Trim());
						}
					} while (true);
					sr.Close();
				}
				catch (Exception ex)
				{
					Console.WriteLine(ex.ToString());
				}
			}
			return rtDataList;
		}
	}
}
