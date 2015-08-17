using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;
using System.Net;

namespace ServiceAreaClientLib
{
	public class PassengerCounterInquirer
	{
		// 要查询的设备列表
        List<PassengerCounterInfo> _deviceList;

		// 数据库服务区情报
		ServerInfo _dbServerInfo;

		public ServerInfo DbServerInfo
		{
			get { return _dbServerInfo; }
			set { _dbServerInfo = value; }
		}

		// 要更新的UI textBox控件
		System.Windows.Forms.TextBox _tbxControl = null;

		public System.Windows.Forms.TextBox TbxControl
		{
			get { return _tbxControl; }
			set { _tbxControl = value; }
		}

		// 循环查询周期(单位为分钟)
		int _cyclePeriod = 10;

		public int CyclePeriod
		{
			get { return _cyclePeriod; }
			set { _cyclePeriod = value; }
		}

        internal List<PassengerCounterInfo> DeviceList
        {
            get { return _deviceList; }
            set { _deviceList = value; }
        }

		public PassengerCounterInquirer(List<PassengerCounterInfo> deviceInfoList, ServerInfo sInfo)
        {
            DeviceList = deviceInfoList;
			DbServerInfo = sInfo;
        }

		System.Timers.Timer _timer;

		/// <summary>
		/// 查询开始
		/// </summary>
		public void StartInquiry()
		{
			// 启动timer
			_timer = new System.Timers.Timer(CyclePeriod * 60 * 1000);
			_timer.AutoReset = false;
			_timer.Elapsed += new System.Timers.ElapsedEventHandler(TimerElapsed);
			_timer.Start();
			// 开始第一次查询
			DoInquiry();
		}

		public void StopInquiry()
		{
			if (null != _timer)
			{
				_timer.Stop();
                DeviceList.Clear();
				_tbxControl = null;
			}
		}

		void TimerElapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			_timer.Start();
			DoInquiry();
		}

        /// <summary>
        /// 查询执行
        /// </summary>
        public void DoInquiry()
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
                if (!Report2Server(dateTimeStr, t0.Result, t1.Result, deviceInfo))
                {
                    AppendUITextBox("	" + deviceInfo.DeviceName + " : 数据库写入失败!");
                }
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

        // 报告给服务器(写入数据库表)
		bool Report2Server(string dateTimeStr, string resultStr0, string resultStr1, PassengerCounterInfo deviceInfo)
        {
			DBConnectMySQL mysql_object = new DBConnectMySQL(DbServerInfo);
			string reportStr0 = GetReportString(resultStr0);
			string reportStr1 = GetReportString(resultStr1);
			// 摄像头的设备种类编码是003
			string deviceTypeStr = "003";
			// 3位服务区编号 + 3位采集点位置编号 + 3位设备种类编号 + 3位设备地址 = 12位设备编号唯一确定一个具体的设备
			// 因为同一个点, 应该只能有一个摄像头, 所以对于摄像头来说, 3位设备地址由0x01固定值取代
			string deviceSnStr = deviceInfo.ServiceArea.ToString().PadLeft(3, '0') + deviceInfo.SpotNumber.PadLeft(3, '0') + deviceTypeStr + 0x01.ToString().PadLeft(3, '0');
            string insertStr = @"INSERT INTO " + deviceInfo.DbTableName + @"(time_stamp, device_number, value_01, value_02" + @") VALUES('"
                                    + dateTimeStr + @"'," + deviceSnStr + reportStr0 + reportStr1 + @")";
			try
			{
				mysql_object.ExecuteMySqlCommand(insertStr);
			}
			catch (Exception ex)
			{
				System.Diagnostics.Trace.WriteLine(ex.ToString());
				return false;
			}
			return true;
        }

        public static string GetReportString(string resultStr)
        {
			string reportStr = "";
			string findKey = "count=";
			int idx;
			if (-1 != (idx = resultStr.LastIndexOf(findKey)) )
			{
				int value;
				string subStr = resultStr.Substring(idx + findKey.Length).Trim();
				if (int.TryParse(subStr, out value))
				{
					reportStr = ", " + subStr;
				}
			}
			return reportStr;
        }

		public delegate void UiUpdateDelegate(string txtStr);

		/// <summary>
		/// 更新UI TextBox控件内容
		/// </summary>
		void AppendUITextBox(string txtStr)
		{
			if (null == _tbxControl)
			{
				return;
			}
			if (_tbxControl.InvokeRequired)
			{
				UiUpdateDelegate updateDel = new UiUpdateDelegate(AppendUITextBox);
				_tbxControl.BeginInvoke(updateDel, new object[] { txtStr });
			}
			else
			{
                if (_tbxControl.Text.Length >= _tbxControl.MaxLength - 1000)
                {
                    _tbxControl.Text = _tbxControl.Text.Substring(_tbxControl.Text.Length - 100);
                }
				_tbxControl.AppendText(txtStr + "\r\n");
			}
		}

	}
}
