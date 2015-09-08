using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;

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

		// 端口号:1981 用于ServiceAreaClient向ServiceAreaServer(中继服务端)发送采集数据report
		// 端口号:1982 用于UpdaterServer向UpdaterClient传送更新文件
		// 端口号:1983 用于ServiceAreaClient监听接收消息
		static int _portTransData = 1982;

		public static int PortTransData
		{
			get { return Program._portTransData; }
			set { Program._portTransData = value; }
		}

		// 更新对象文件名
        static string _updateFileName = @".\ServiceAreaClient.exe";

		public static string UpdateFileName
		{
			get { return Program._updateFileName; }
			set { Program._updateFileName = value; }
		}

		// 更新传输临时文件名
		static string _tempFileName = @".\temp.dat";

		public static string TempFileName
		{
			get { return Program._tempFileName; }
			set { Program._tempFileName = value; }
		}

        // 可执行文件名
        static string _exeFileName = @".\ServiceAreaClient.exe";

        public static string ExeFileName
        {
            get { return Program._exeFileName; }
            set { Program._exeFileName = value; }
        }

		#endregion
		static void Main(string[] args)
		{
			string cmdStr = "";
			if (args.Length > 0)
			{
				string paraStr = args[0];
                Console.WriteLine(args[0]);
                if (args.Length > 1)
                {
                    Console.WriteLine(args[1]);
                }
				string[] paras = paraStr.Split(',');
				if (paras.Length >= 2)
				{
					Host = paras[0];
					cmdStr = paras[1];
					if (cmdStr.Trim().ToLower().Equals("update exe"))
					{
						UpdateFileName = "ServiceAreaClient.exe";
					}
                    else if (cmdStr.Trim().ToLower().Equals("update dll"))
                    {
                        UpdateFileName = "ServiceAreaClientLib.dll";
                    }
					else if (cmdStr.Trim().ToLower().Equals("update setting"))
					{
						UpdateFileName = "data.xml";
					}
					else if (cmdStr.Trim().ToLower().Equals("update config"))
					{
						UpdateFileName = "config.ini";
					}
				}
			}
			IPEndPoint ipep = new IPEndPoint(IPAddress.Parse(Host), PortTransData);
			Socket sServer = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

			try
			{
				Thread.Sleep(3000);
				sServer.Connect(ipep);

				// 向服务器发消息表示更新开始准备好
				string sndStr = "Update Start Ready";
				byte[] sndBytes = Encoding.ASCII.GetBytes(sndStr);
				sServer.Send(sndBytes);
				Console.WriteLine("更新开始就绪送信");

				// 开始接收文件数据
				FileDataRecv(GetModulePath() + TempFileName, sServer);
                sServer.Close();

                sServer = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                sServer.Connect(ipep);
                sndStr = "Update File Receive OK";
                sndBytes = Encoding.ASCII.GetBytes(sndStr);
                sServer.Send(sndBytes);
                Console.WriteLine(sndStr);
                Thread.Sleep(1000);

				// 确认本地程序已关闭

				// 复制更新文件
				File.Copy(TempFileName, UpdateFileName, true);
                sndStr = "Update File Copy OK";
                sndBytes = Encoding.ASCII.GetBytes(sndStr);
                sServer.Send(sndBytes);
                Console.WriteLine(sndStr);
				Thread.Sleep(1000);

                // 删除临时文件
                File.Delete(TempFileName);
				sndStr = "Temporary File Delete OK";
                sndBytes = Encoding.ASCII.GetBytes(sndStr);
                sServer.Send(sndBytes);
                Console.WriteLine(sndStr);
                Thread.Sleep(1000);

                sndStr = "Start Loading the Updated Program...";
                sndBytes = Encoding.ASCII.GetBytes(sndStr);
                sServer.Send(sndBytes);
                Console.WriteLine(sndStr);
				// 重新启动程序
				System.Diagnostics.Process exep = new System.Diagnostics.Process();
                exep.StartInfo.FileName = ExeFileName;
				exep.Start();

                sndStr = "UpdaterClient.exe Close";
                sndBytes = Encoding.ASCII.GetBytes(sndStr);
                sServer.Send(sndBytes);
                sServer.Close();
            }
			catch (Exception ex)
			{
				Console.WriteLine(ex.ToString());
                Console.ReadKey();
			}
			finally
			{
			}
			Console.WriteLine("UpdaterClient任务结束,准备退出!");
			Thread.Sleep(3000);
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
