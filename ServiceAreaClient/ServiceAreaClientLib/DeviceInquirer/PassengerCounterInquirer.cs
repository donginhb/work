using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;
using System.Net;

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

				// 异步执行两个查询
				Task<string> t0 = GetResultStringAsync(deviceInfo.RequestString1);
				Task<string> t1 = GetResultStringAsync(deviceInfo.RequestString2);
				AppendUITextBox("	" + deviceInfo.DeviceName + " 返回应答: " + t0.Result);
				AppendUITextBox("	" + deviceInfo.DeviceName + " 返回应答: " + t1.Result);

				string insertStr = GetInsertString(dateTimeStr, t0.Result, t1.Result, deviceInfo);
				Report2Server(insertStr, deviceInfo.DeviceName);
			}
			catch (Exception ex)
			{
				AppendUITextBox("	" + deviceInfo.DeviceName + ": 查询失败!");
				System.Diagnostics.Trace.WriteLine(ex.ToString());
			}
        }

		async Task<string> GetResultStringAsync(string requestStr)
		{
			WebClient wc = new WebClient();
			string resultStr = await wc.DownloadStringTaskAsync(new Uri(requestStr));

			return resultStr;
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

		public static string GetInsertString(string dateTimeStr, string resultStr0, string resultStr1, PassengerCounterInfo deviceInfo)
		{
			string reportStr0 = GetReportString(resultStr0, deviceInfo.RequestString1);
			string reportStr1 = GetReportString(resultStr1, deviceInfo.RequestString2);
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
