using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;

namespace UpdaterServer
{
	class Program
	{
		#region 全部字段

		// 端口号:1981 用于ServiceAreaClient向ServiceAreaServer(中继服务端)发送采集数据report
		// 端口号:1982 用于UpdaterServer向UpdaterClient传送更新文件
		// 端口号:1983 用于ServiceAreaClient监听接收消息
		static int _portTransData = 1982;

		public static int PortTransData
		{
			get { return _portTransData; }
			set { _portTransData = value; }
		}

		static int _portListener = 1983;

		public static int PortListener
		{
			get { return Program._portListener; }
			set { Program._portListener = value; }
		}

		// 文件传送时的包的大小(10KB)
		static int _packetSize = 10 * 1024;

		public static int PacketSize
		{
			get { return Program._packetSize; }
			set { Program._packetSize = value; }
		}

		static string _updateFileName = @"\Send\dummy.exe";

		public static string UpdateFileName
		{
			get { return Program._updateFileName; }
			set { Program._updateFileName = value; }
		}

		#endregion

		static void Main(string[] args)
		{
			// 获得输入的对端IP地址
			IPAddress clientIpAddr = GetTargetIpAddr();

			Console.WriteLine("更新开始: " + clientIpAddr.ToString());
			// 首先以客户端的身份连接ServiceAreaClient的消息监听线程, 并向其发送更新指示"Update Program"
			IPEndPoint ipep = new IPEndPoint(clientIpAddr, PortListener);
			Socket cSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

			try
			{
				cSocket.Connect(ipep);
				Console.WriteLine("Connect成功!");

				// 向客户端发送更新指示
				string sndStr = "Update Program";
				byte[] sndBytes = Encoding.ASCII.GetBytes(sndStr);
				cSocket.Send(sndBytes);
				Console.WriteLine("\"Update Program\"送信成功!");
				cSocket.Close();

				// 接着再以服务器的身份等待来自客户端更新程序(UpdaterClient)的更新就绪应答
				ipep = new IPEndPoint(IPAddress.Any, PortTransData);
				Socket sSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
				sSocket.Bind(ipep);												// 绑定
				sSocket.Listen(10);												// 监听

				Console.WriteLine(@"等待客户端更新程序的连接...");
				cSocket = sSocket.Accept();							// 当有可用的客户端连接尝试时执行，并返回一个新的socket,用于与客户端之间的通信
				IPEndPoint clientip = (IPEndPoint)cSocket.RemoteEndPoint;
				Console.WriteLine(@"与客户端更新程序建立连接: " + clientip.Address + @" 端口号: " + clientip.Port);

				UpdateProcess(cSocket);

				Console.WriteLine("客户端: " + clientip.Address + "更新OK!");
				cSocket.Close();
				sSocket.Close();
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.ToString());
			}
			finally
			{
			}

			Console.WriteLine("UpdaterServer结束退出!");
			Thread.Sleep(3000);
		}

		#region 内部方法

		static void UpdateProcess(Socket cSocket)
		{
			string recvStr = "";
			byte[] recvBytes = new byte[1024];
			int bytes;
			bytes = cSocket.Receive(recvBytes, recvBytes.Length, 0);	// 从客户端接受消息

			recvStr += Encoding.ASCII.GetString(recvBytes, 0, bytes);
			Console.WriteLine("Server get message:{0}", recvStr);		// 把客户端传来的信息显示出来

			// 处理客户端发来的消息
			ClientMsgProcess(recvStr, cSocket);
		}

		private static bool ClientMsgProcess(string clientMsg, Socket s)
		{
			if ("Update Start Ready" == clientMsg)
			{
				// 开始进行更新文件的传输
				FileDataSend(GetModulePath() + UpdateFileName, s);
				return true;
			}
			return false;
		}

		static string GetModulePath()
		{
			string retStr = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
			int idx = -1;
			if (-1 != (idx = retStr.LastIndexOf(@"\")))
			{
				retStr = retStr.Remove(idx);
			}
			return retStr;
		}

		static int SendVarData(Socket s, byte[] data) // return integer indicate how many data sent.
		{
			int total = 0;
			int size = data.Length;
			int dataleft = size;
			int sent;
			byte[] datasize = new byte[4];
			datasize = BitConverter.GetBytes(size);
			sent = s.Send(datasize);						// send the size of data array.

			while (total < size)
			{
				sent = s.Send(data, total, dataleft, SocketFlags.None);
				total += sent;
				dataleft -= sent;
			}

			return total;  
		}

		static void FileDataSend(string full_name, Socket sClient)
		{
			FileInfo sfi = new FileInfo(full_name);
			if (!sfi.Exists)
			{
				return;
			}
			FileStream sfs = sfi.OpenRead();

			int sndPacket = (int)(sfs.Length / PacketSize);
			int lastPacket = (int)(sfs.Length - sndPacket * PacketSize);

			byte[] data = new byte[PacketSize];
			for (int i = 0; i < sndPacket; i++)
			{
				sfs.Read(data, 0, data.Length);
				SendVarData(sClient, data);
			}

			if (lastPacket != 0)
			{
				data = new byte[lastPacket];
				sfs.Read(data, 0, data.Length);
				SendVarData(sClient, data);
			}

			sfs.Close();
		}

		/// <summary>
		/// 取得更新对端的IP地址输入
		/// </summary>
		/// <returns></returns>
		static IPAddress GetTargetIpAddr()
		{
			string clientIpStr = "";
			IPAddress clientIpAddr;
			while (true)
			{
				Console.WriteLine("请输入更新对端的IP地址:");
				clientIpStr = Console.ReadLine();
				if (!IPAddress.TryParse(clientIpStr, out clientIpAddr))
				{
					Console.WriteLine("输入的IP地址不正确");
				}
				else
				{
					break;
				}
			}
			return clientIpAddr;
		}

		#endregion
	}
}
