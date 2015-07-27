using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;
using System.Net;

namespace ServiceAreaClientLib
{
	public class HttpDeviceInquirer
	{
		// 要查询的设备列表
        List<HttpDeviceInfo> _deviceList;

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

        internal List<HttpDeviceInfo> DeviceList
        {
            get { return _deviceList; }
            set { _deviceList = value; }
        }

		public HttpDeviceInquirer(List<HttpDeviceInfo> deviceInfoList)
        {
            DeviceList = deviceInfoList;
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
				_deviceList.Clear();
				_tbxControl = null;
			}
		}

		void TimerElapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			_timer.Start();
			DoInquiry();
		}

        /// <summary>
        /// 电表查询执行
        /// </summary>
        public void DoInquiry()
        {
            if (null != _deviceList)
            {
                // 对列表中的各个设备, 逐一进行查询
                for (int i = 0; i < _deviceList.Count; i++)
                {
					HttpDeviceInfo di = _deviceList[i];
					AppendUITextBox("开始查询 " + di.Name);
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
        void InquiryTask(HttpDeviceInfo deviceInfo)
        {
			try
			{
				WebClient wc = new WebClient();
				string resultStr = wc.DownloadString(new Uri(deviceInfo.RequestString));
				AppendUITextBox("	" + deviceInfo.Name + " 返回应答: " + resultStr);
			}
			catch (Exception ex)
			{
				AppendUITextBox("	" + deviceInfo.Name + ": 查询失败!");
			}
        }

        void Report2Server(InquiryResult inquiryResult)
        {
        }

        public static string GetReportString(InquiryResult inquiryResult)
        {
			string reportStr = "";
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
				_tbxControl.AppendText(txtStr + "\r\n");
			}
		}

	}
}
