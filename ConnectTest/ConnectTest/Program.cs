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
                // 水表
				Connect("10.62.1.206", 26);
                // 锅炉温度
                // Connect("10.62.1.204", 26);
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

//			byte[] sendBytes = { 0x10, 0x07, 0x00, 0x00, 0x00, 0x01, 0x0a };
            // 水表
            byte[] tmpBytes = { 0x08, 0x03, 0x02, 0x02, 0x00, 0x02 };
            // 锅炉温度
            //byte[] tmpBytes = { 0x08, 0x03, 0x00, 0x01, 0x00, 0x01 };

            UInt16 crc16 = CRC16(tmpBytes, 6);
            byte crcLowByte = (byte)(crc16 & 0x00FF);
            byte crcHighByte = (byte)((crc16 & 0xFF00) >> 8);
            // 水表
            byte[] sendBytes = { 0x08, 0x03, 0x02, 0x02, 0x00, 0x02, crcLowByte, crcHighByte };
            // 锅炉水温
            //byte[] sendBytes = { 0x08, 0x03, 0x00, 0x01, 0x00, 0x01, crcLowByte, crcHighByte };

			for (int i = 0; i < 1; i++)
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

        public static UInt16 CRC16(Byte[] dat, int count)
        {
            uint crc = 0xFFFF;
            int i, j;
            for (j = 0; j < count; j++)
            {
                crc = crc ^ dat[j];
                for (i = 0; i < 8; i++)
                {
                    if ((crc & 0x0001) > 0)
                    {
                        crc = crc >> 1;
                        crc = crc ^ 0xa001;
                    }
                    else
                    {
                        crc = crc >> 1;
                    }
                }
            }
            return (UInt16)(crc);
        }

	}
}
