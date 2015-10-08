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
			string requestStr = @"http://10.62.72.116/nvc-cgi/admin/vca.cgi?action=list&amp;group=VCA.Ch0.Ct0.count";
			int idx = requestStr.LastIndexOf("=");
			if (-1 == idx)
			{
				return;
			}
			string findKey = requestStr.Substring(idx + 1).Trim() + "=";
			string rdStr = "";
			string reportStr = "";

			HttpWebRequest hwreq = null;
			WebResponse hwrsp = null;
			Stream rspStream = null;
			StreamReader sr = null;
			try
			{
				hwreq = (HttpWebRequest)WebRequest.Create(requestStr);
				hwrsp = hwreq.GetResponse();
				rspStream = hwrsp.GetResponseStream();
				sr = new StreamReader(rspStream);
				while ( null != (rdStr = sr.ReadLine()) )
				{
					// Console.WriteLine(rdStr);
					if ( -1 != (idx = rdStr.IndexOf(findKey)) )
					{
						string subStr = rdStr.Substring(idx + findKey.Length).Trim();
						int value;
						if (int.TryParse(subStr, out value))
						{
							reportStr = ", " + subStr;
							Console.WriteLine("reportStr = \"" + reportStr + "\"");
							break;
						}
					}
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.ToString());
			}
			finally
			{
				if (null != hwrsp)
				{
					hwrsp.Close();
				}
				if (null != rspStream)
				{
					rspStream.Close();
				}
				if (null != sr)
				{
					sr.Close();
				}
			}
			//string resultStr = GetReportString(outStr, requestStr);
			//Console.WriteLine(resultStr);

			Console.ReadLine();
		}

		public static string GetReportString(string resultStr, string requestStr)
		{
			string reportStr = "";
			int idx = requestStr.LastIndexOf("=");
			string findKey = "";
			int value;
			if (-1 != idx)
			{
				findKey = requestStr.Substring(idx + 1).Trim() + "=";
				string[] rdLines = resultStr.Split('\n');
				foreach (string line in rdLines)
				{
					if ("" == line.Trim())
					{
						continue;
					}
					if (-1 != (idx = line.IndexOf(findKey)))
					{
						string subStr = line.Substring(idx + findKey.Length).Trim();
						if (int.TryParse(subStr, out value))
						{
							reportStr = ", " + subStr;
							break;
						}
					}
				}
			}

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
