using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;
using System.IO;

namespace ConnectTest
{
	class Program
	{
		static void Main(string[] args)
		{
			Console.WriteLine("Hello World!");
			string outStr = "";
			string requestStr = @"http://10.62.72.116/nvc-cgi/admin/vca.cgi?action=list&amp;group=VCA.Ch0.Ct0.count";
			try
			{
				Task<string> t0 = GetResultStringAsync(requestStr);
				outStr = t0.Result;
				StreamWriter sw = new StreamWriter("dump.txt", false);
				sw.Write(outStr);
				sw.Close();
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.ToString());
			}
			string resultStr = GetReportString(outStr);
			//Console.WriteLine(resultStr);

			Console.ReadLine();
		}

		async static Task<string> GetResultStringAsync(string requestStr)
		{
			WebClient wc = new WebClient();
			string resultStr = await wc.DownloadStringTaskAsync(new Uri(requestStr));

			return resultStr;
		}

		public static string GetReportString(string resultStr)
		{
			string reportStr = "";
			string findKey = "count=";
			int idx;
			int value = 0;
			string subStr = "";
			if (-1 != (idx = resultStr.LastIndexOf(findKey)))
			{
				subStr = resultStr.Substring(idx + findKey.Length).Trim();
				if (int.TryParse(subStr, out value))
				{
					reportStr = ", " + subStr;
				}
			}
			Console.WriteLine("idx = " + idx.ToString() + " value = " + value.ToString() + ", subStr.Length = " + subStr.Length.ToString());
			subStr = subStr.Substring(0, 100);
			Console.WriteLine(subStr);
			return reportStr;
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
