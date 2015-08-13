using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;
using System.Net;

namespace ServiceAreaClientLib.DeviceInquirer
{
	public class ModbusDeviceInquirer
	{
		// 要查询的设备列表
		protected List<ModbusDeviceInfo> _deviceList;

		public List<ModbusDeviceInfo> DeviceList
		{
			get { return _deviceList; }
			set { _deviceList = value; }
		}

		// 数据库服务区情报
		protected ServerInfo _dbServerInfo;

		public ServerInfo DbServerInfo
		{
			get { return _dbServerInfo; }
			set { _dbServerInfo = value; }
		}

		// 要更新的UI textBox控件
		protected System.Windows.Forms.TextBox _tbxControl = null;

		public System.Windows.Forms.TextBox TbxControl
		{
			get { return _tbxControl; }
			set { _tbxControl = value; }
		}

		// 循环查询周期(单位为分钟)
		protected int _cyclePeriod = 10;

		public int CyclePeriod
		{
			get { return _cyclePeriod; }
			set { _cyclePeriod = value; }
		}

		protected System.Timers.Timer _timer;

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

		protected void TimerElapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			_timer.Start();
			DoInquiry();
		}

        /// <summary>
        /// 查询执行
        /// </summary>
        public void DoInquiry()
        {
			if ((null != DeviceList)
				&& (0 != DeviceList.Count))
			{
				AppendUITextBox("\r\n>------------------------------->");
				AppendUITextBox(DateTime.Now.ToLongDateString() + " " + DateTime.Now.ToLongTimeString());
				// 对列表中的各个设备, 逐一进行查询
				for (int i = 0; i < DeviceList.Count; i++)
				{
					ModbusDeviceInfo di = DeviceList[i];
					AppendUITextBox("开始查询 " + di.DeviceSn);
					Thread inquiryThread = new Thread(delegate() { InquiryTask(di); });
					inquiryThread.Start();
					System.Threading.Thread.Sleep(500);
				}
			}
		}

		void InquiryTask(ModbusDeviceInfo deviceInfo)
		{
		}

		public delegate void UiUpdateDelegate(string txtStr);

		/// <summary>
		/// 更新UI TextBox控件内容
		/// </summary>
		protected void AppendUITextBox(string txtStr)
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
