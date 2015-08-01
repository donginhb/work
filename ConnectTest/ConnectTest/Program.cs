using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;

namespace ConnectTest
{
	class Program
	{
		static void Main(string[] args)
		{
			Console.WriteLine("Hello World!");
			try
			{
				Connect("192.168.0.7", 26);
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.ToString());
			}
			Console.WriteLine("Connect Success!");


			Console.ReadLine();
		}

		public static void Connect(string host, int port)
		{
			IPAddress ip = IPAddress.Parse(host);
			IPEndPoint ipe = new IPEndPoint(ip, port);

			Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			socket.Connect(ipe);

			byte[] sendBytes = { 0x10, 0x07, 0x00, 0x00, 0x00, 0x01, 0x0a };
			for (int i = 0; i < 3; i++)
			{
				socket.Send(sendBytes);

				byte[] rcvBytes = new byte[1024];
				int bytes = socket.Receive(rcvBytes, rcvBytes.Length, 0);
				string outStr = System.Text.Encoding.ASCII.GetString(rcvBytes);

				Console.WriteLine("Receive " + bytes.ToString() + " bytes!");
				Console.WriteLine(outStr);

				System.Threading.Thread.Sleep(800);
			}
		}

	}
}
