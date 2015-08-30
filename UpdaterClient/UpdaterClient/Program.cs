using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;
using System.IO;

namespace UpdaterClient
{
	class Program
	{
		#region 字段
		static string _host = "127.0.0.1";

		public static string Host
		{
			get { return Program._host; }
			set { Program._host = value; }
		}

		static int _port = 1982;

		public static int Port
		{
			get { return Program._port; }
			set { Program._port = value; }
		}

		#endregion
		static void Main(string[] args)
		{
			IPEndPoint ipep = new IPEndPoint(IPAddress.Parse(Host), Port);
			Socket sServer = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

			try
			{
				sServer.Connect(ipep);

				// 向服务器发消息表示更新开始准备好
				string sndStr = "Update Start Ready";
				byte[] sndBytes = Encoding.ASCII.GetBytes(sndStr);
				sServer.Send(sndBytes);

				// 开始接收文件数据
				FileDataRecv(GetModulePath() + @"\Recv\test.pdf", sServer);

				// 确认本地程序已关闭

				// 复制更新文件

				// 重新启动程序
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.ToString());
			}
			finally
			{
			}
			Console.WriteLine("Client到最后了!");
			Console.ReadLine();
		}

		#region 内部方法
		static void FileDataRecv(string full_name, Socket sServer)
		{
			FileStream MyFileStream = new FileStream(full_name, FileMode.Create, FileAccess.Write);

			while (true)
			{
				byte[] data = ReceiveVarData(sServer);
				if(data.Length == 0)
				{
					break;
				}
				else
				{
					MyFileStream.Write(data, 0, data.Length);
				}
			}

			MyFileStream.Close();
			sServer.Close();
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

		static byte[] ReceiveVarData(Socket s)			// return array that store the received data.
		{
			int total = 0;
			int recv;
			byte[] datasize = new byte[4];
			recv = s.Receive(datasize, 0, 4, SocketFlags.None);	// receive the size of data array for initialize a array.
			int size = BitConverter.ToInt32(datasize, 0);
			int dataleft = size;
			byte[] data = new byte[size];

			while (total < size)
			{
				recv = s.Receive(data, total, dataleft, SocketFlags.None);
				if (recv == 0)
				{
					data = null;
					break;
				}
				total += recv;
				dataleft -= recv;
			}

			return data;
		}

		#endregion
	}
}
