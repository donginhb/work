using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;

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

        static int _port = 1981;

        public static int Port
        {
            get { return Program._port; }
            set { Program._port = value; }
        }

		static void Main(string[] args)
		{
			// 读设定文件
			LoadIniFile();

			byte[] data = new byte[1024];									// 用于缓存客户端所发送的信息,通过socket传递的信息必须为字节数组
			IPEndPoint ipep = new IPEndPoint(IPAddress.Any, Port);			// 本机预使用的IP和端口
			Socket sSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

			try
			{
				sSocket.Bind(ipep);												// 绑定
				sSocket.Listen(10);												// 监听

				// 第一层的主循环, 循环接收来自各个客户端的数据
				while (true)
				{
					Console.WriteLine("Waiting for a client");
					Socket cSocket = sSocket.Accept();							// 当有可用的客户端连接尝试时执行，并返回一个新的socket,用于与客户端之间的通信
					IPEndPoint clientip = (IPEndPoint)cSocket.RemoteEndPoint;
					Console.WriteLine("Connect with client:" + clientip.Address + " at port:" + clientip.Port);

					string recvStr = "";
					byte[] recvBytes = new byte[1024];
					int bytes;
					bytes = cSocket.Receive(recvBytes, recvBytes.Length, 0);	// 从客户端接受消息
					recvStr += Encoding.ASCII.GetString(recvBytes, 0, bytes);
					Console.WriteLine("Server get message:{0}", recvStr);		// 把客户端传来的信息显示出来

					string sendStr = "";
					// 处理客户端发来的消息
					if (ClientMsgProcess(recvStr))
					{
						Console.WriteLine("☆☆☆ Process OK! ☆☆☆");
						sendStr = "Server receive OK!";
					}
					else
					{
						Console.WriteLine("※※※ Process Fail! ※※※");
						sendStr = "Server receive Fail!";
					}

					// 给Client端返回信息
					byte[] bs = Encoding.ASCII.GetBytes(sendStr);
					//cSocket.Send(bs, bs.Length, 0);  //返回信息给客户端
					cSocket.Close();

					// System.Threading.Thread.Sleep(1000);
				}
			}
			catch (Exception ex)
			{
                Console.WriteLine(ex.ToString());
			}
			finally
			{
				sSocket.Close();
			}
		}

		private static bool ClientMsgProcess(string clientMsg)
		{
			// 执行MySQL命令
			DBConnectMySQL mysql_object = new DBConnectMySQL(DbServerInfo);
			try
			{
				mysql_object.ExecuteMySqlCommand(clientMsg);
			}
			catch (Exception ex)
			{
				System.Diagnostics.Trace.WriteLine(ex.ToString());
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
	}
}
