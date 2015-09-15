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
			// 执行MySQL命令
			if (WriteToDB(clientMsg))
			{
				// 数据库保存成功
				DataBaseConnected = true;
				Console.WriteLine("☆☆☆ Save To DB Success! ☆☆☆");
				return true;
			}
			else
			{
				// 数据库保存失败
				DataBaseConnected = false;
				Thread th = new Thread(delegate() { SaveToLocalFile(clientMsg); });
				th.Start();
				Console.WriteLine("※※※ Save To DB Fail! ※※※");
				return false;
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

		protected static void SaveToLocalFile(string cmdStr)
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

		protected static void CheckLocalBuffer()
		{
			if (DataBaseConnected)
			{
				// 只有在前一次数据库连接为切断状态时才进行处理
				return;
			}
			if (File.Exists(LocalBufFileName))
			{
				lock (bufferLock)
				{
					try
					{
						StreamReader sr = new StreamReader(LocalBufFileName);
						string rdLine = "";
						List<string> cmdList = new List<string>();
						while (null != (rdLine = sr.ReadLine()))
						{
							if (string.Empty != rdLine.Trim())
							{
								cmdList.Add(rdLine);
							}
						}
						sr.Close();
						File.Delete(LocalBufFileName);
						List<string> failList = new List<string>();
						foreach (string cmd in cmdList)
						{
							if (!ClientMsgProcess(cmd))
							{
								failList.Add(cmd);
							}
						}
						// 失败的话, 再存回本地缓存文件
						if (0 != failList.Count)
						{
							StreamWriter sw = new StreamWriter(LocalBufFileName, true);
							foreach (string cmd in failList)
							{
								sw.WriteLine(cmd);
							}
							sw.Close();
						}
					}
					catch (Exception ex)
					{
						Console.WriteLine(ex.ToString());
					}
				}
			}
		}

	}
}
