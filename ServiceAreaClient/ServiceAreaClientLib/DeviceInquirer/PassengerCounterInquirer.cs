using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;
using System.Net;
using System.IO;

namespace ServiceAreaClientLib.DeviceInquirer
{
	public class PassengerCounterInquirer : DeviceInquirer
	{
		// 要查询的设备列表
        List<PassengerCounterInfo> _deviceList;

        internal List<PassengerCounterInfo> DeviceList
        {
            get { return _deviceList; }
            set { _deviceList = value; }
        }

		public PassengerCounterInquirer(List<PassengerCounterInfo> deviceInfoList, ServerInfo dbServer,
										ServerInfo relayServer, E_DB_CONNECT_MODE dbConnectMode)
        {
            DeviceList = deviceInfoList;
			DbServerInfo = dbServer;
			RelayServerInfo = relayServer;
			Db_connect_mode = dbConnectMode;
        }

        /// <summary>
        /// 查询执行
        /// </summary>
        override public void DoInquiry()
        {
            if (    (null != DeviceList)
                &&  (0 != DeviceList.Count) )
            {
                AppendUITextBox("\r\n>------------------------------->");
                AppendUITextBox(DateTime.Now.ToLongDateString() + " " + DateTime.Now.ToLongTimeString());
                // 对列表中的各个设备, 逐一进行查询
                for (int i = 0; i < DeviceList.Count; i++)
                {
					if (0 == i)
					{
						// 检查有没有缓存数据
						CheckLocalBuffer();
					}
                    PassengerCounterInfo di = DeviceList[i];
					AppendUITextBox("开始查询 " + di.DeviceName);
					Thread inquiryThread = new Thread(delegate() { InquiryTask(di); });
					inquiryThread.Start();
					System.Threading.Thread.Sleep(300);
                }
            }
        }

		/// <summary>
		/// 单个设备查询线程的执行过程
		/// </summary>
		/// <param name="deviceInfo"></param>
        void InquiryTask(PassengerCounterInfo deviceInfo)
        {
			try
			{
                string dateTimeStr = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");

				string pc0Str = GetPassengerCountStr(deviceInfo.RequestString1);
				AppendUITextBox("	" + deviceInfo.DeviceName + " 返回应答: " + pc0Str);
				string pc1Str = GetPassengerCountStr(deviceInfo.RequestString2);
				AppendUITextBox("	" + deviceInfo.DeviceName + " 返回应答: " + pc1Str);

				if (	string.Empty != pc0Str
					&&	string.Empty != pc1Str	)
				{
					string insertStr = GetInsertString(dateTimeStr, pc0Str, pc1Str, deviceInfo);
					Report2Server(insertStr, deviceInfo.DeviceName);
				}
			}
			catch (Exception ex)
			{
				AppendUITextBox("	" + deviceInfo.DeviceName + ": 查询失败!");
				System.Diagnostics.Trace.WriteLine(ex.ToString());
			}
        }

		/// <summary>
		/// 取得客流数计数器的值
		/// </summary>
		/// <param name="reqStr"></param>
		/// <returns></returns>
		string GetPassengerCountStr(string reqStr)
		{
			int idx = reqStr.LastIndexOf("=");
			if (-1 == idx)
			{
				return string.Empty;
			}
			string findKey = reqStr.Substring(idx + 1).Trim() + "=";
			string rdStr = "";
			string retStr = string.Empty;

			HttpWebRequest hwreq = null;
			WebResponse hwrsp = null;
			Stream rspStream = null;
			StreamReader sr = null;
			try
			{
				hwreq = (HttpWebRequest)WebRequest.Create(reqStr);
				hwrsp = hwreq.GetResponse();
				rspStream = hwrsp.GetResponseStream();
				sr = new StreamReader(rspStream);
				while (null != (rdStr = sr.ReadLine()))
				{
					if (-1 != (idx = rdStr.IndexOf(findKey)))
					{
						string subStr = rdStr.Substring(idx + findKey.Length).Trim();
						int value;
						if (int.TryParse(subStr, out value))
						{
							retStr = subStr;
							break;
						}
					}
				}
			}
			catch (Exception ex)
			{
				System.Diagnostics.Trace.WriteLine(ex.ToString());
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
			return retStr;
		}

		public static string GetInsertString(string dateTimeStr, string resultStr0, string resultStr1, PassengerCounterInfo deviceInfo)
		{
			string reportStr0 = ", " + resultStr0;
			string reportStr1 = ", " + resultStr1;
			// 摄像头的设备种类编码是003
			string deviceTypeStr = "003";
			// 3位服务区编号 + 3位采集点位置编号 + 3位设备种类编号 + 3位设备地址 = 12位设备编号唯一确定一个具体的设备
			// 因为同一个点, 应该只能有一个摄像头, 所以对于摄像头来说, 3位设备地址由0x01固定值取代
			string deviceSnStr = deviceInfo.ServiceArea.ToString().PadLeft(3, '0') + deviceInfo.SpotNumber.PadLeft(3, '0') + deviceTypeStr + 0x01.ToString().PadLeft(3, '0');
			string insertStr = @"INSERT INTO " + deviceInfo.DbTableName + @"(time_stamp, device_number, value_01, value_02" + @") VALUES('"
									+ dateTimeStr + @"'," + deviceSnStr + reportStr0 + reportStr1 + @")";

			return insertStr;
		}

	}
}
