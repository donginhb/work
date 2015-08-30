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
		#region 字段
		// 端口号
		static int _port = 1982;

		public static int Port
		{
			get { return _port; }
			set { _port = value; }
		}

		// 文件传送时的包的大小(10KB)
		static int _packetSize = 10 * 1024;

		public static int PacketSize
		{
			get { return Program._packetSize; }
			set { Program._packetSize = value; }
		}

		#endregion

		static void Main(string[] args)
		{
			IPEndPoint ipep = new IPEndPoint(IPAddress.Any, Port);			// 本机预使用的IP和端口
			Socket sSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

			try
			{
				sSocket.Bind(ipep);												// 绑定
				sSocket.Listen(10);												// 监听

				// 第一层的主循环, 接收来自各个客户端的数据
				while (true)
				{
					Console.WriteLine(@"等待客户端的连接...");
					Socket cSocket = sSocket.Accept();							// 当有可用的客户端连接尝试时执行，并返回一个新的socket,用于与客户端之间的通信
					IPEndPoint clientip = (IPEndPoint)cSocket.RemoteEndPoint;
					Console.WriteLine(@"与客户端建立连接: " + clientip.Address + @" 端口号: " + clientip.Port);

					UpdateProcess(cSocket);

					Console.WriteLine("客户端: " + clientip.Address + "更新OK!");
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

			Console.WriteLine("Server到最后了!");
			Console.ReadLine();
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
				FileDataSend(GetModulePath() + @"\Send\test.pdf", s);
				s.Close();
			}
			return true;
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

		#endregion
	}
}
