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

		static Queue<string> _sendBufferQueue = new Queue<string>();

		public static Queue<string> SendBufferQueue
		{
			get { return Program._sendBufferQueue; }
			set { Program._sendBufferQueue = value; }
		}

		static System.Timers.Timer _sendTimer;

		public static System.Timers.Timer SendTimer
		{
			get { return _sendTimer; }
			set { _sendTimer = value; }
		}

		static void Main(string[] args)
		{
			// 读设定文件
			LoadIniFile();

			SendTimer = new System.Timers.Timer(1000);
			SendTimer.AutoReset = false;
			SendTimer.Elapsed += SendTimer_Elapsed;
			SendTimer.Start();

			IPEndPoint ipep = new IPEndPoint(IPAddress.Any, Port);				// 本机预使用的IP和端口
			Socket sSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

			sSocket.Bind(ipep);													// 绑定
			sSocket.Listen(100);												// 监听

			// 第一层的主循环, 循环接收来自各个客户端的数据
			while (true)
			{
				try
				{
					Console.WriteLine("ServiceArea Relay Server(2015_11_15)");
					Console.WriteLine("Waiting for a client...");
					Socket cSocket = sSocket.Accept();							// 当有可用的客户端连接尝试时执行，并返回一个新的socket,用于与客户端之间的通信
					IPEndPoint clientip = (IPEndPoint)cSocket.RemoteEndPoint;
					Console.WriteLine("Connect with client:" + clientip.Address + " at port:" + clientip.Port);

                    LogOutput.LogAppend("");

					// 第二层循环, 接收某个客户端连接的全部数据
					while (true)
					{
						string recvStr = string.Empty;
						byte[] recvBytes = new byte[1024];
						int bytes;
						bytes = cSocket.Receive(recvBytes, recvBytes.Length, 0);// 从客户端接受消息
						if (bytes > 0)
						{
							recvStr += Encoding.UTF8.GetString(recvBytes, 0, bytes);
							Console.WriteLine("Server get message:{0}", recvStr);// 把客户端传来的信息显示出来
							// 加入到发送队列里
							lock (SendBufferQueue)
							{
								SendBufferQueue.Enqueue(recvStr);
                                LogOutput.LogAppend("Enqueue : " + recvStr);
							}
						}
						else
						{
							break;
						}
					}
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
		}

		static void SendTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			while (true)
			{
				lock (SendBufferQueue)
				{
					if (0 == SendBufferQueue.Count)
					{
						break;
					}
					string dataStr = SendBufferQueue.Dequeue();
					SendClientData2DB(dataStr);
				}
			}
			SendTimer.Start();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="clientDataStr"></param>
		/// <param name="isReSend">标识是否是重发</param>
		/// <returns></returns>
		private static bool SendClientData2DB(string clientDataStr)
		{
			// 执行MySQL命令
			if (WriteToDB(clientDataStr))
			{
				// 数据库保存成功
				Console.WriteLine("☆☆☆ Save To DB Success! ☆☆☆");
                LogOutput.LogAppend("SendClientData2DB Success : " + clientDataStr);
                return true;
			}
			else
			{
				// 数据库保存失败
				Console.WriteLine("※※※ Save To DB Fail! ※※※");
                LogOutput.LogAppend("SendClientData2DB Fail : " + clientDataStr);
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
                LogOutput.LogAppend(ex.ToString());
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

            string logOutputEnableStr = IniFile.IniReadValue("LOG_OUTPUT", "ENABLE");
            bool logOutputEnable = false;
            if (bool.TryParse(logOutputEnableStr, out logOutputEnable))
            {
                LogOutput.Enabled = logOutputEnable;
            }
		}

	}
}
