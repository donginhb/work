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
	public class ModbusDeviceInquirer : DeviceInquirer
	{
		// 要查询的设备列表
		protected List<ModbusDeviceInfo> _deviceList;

		public List<ModbusDeviceInfo> DeviceList
		{
			get { return _deviceList; }
			set { _deviceList = value; }
		}

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
					AppendUITextBox("开始查询 " + di.DeviceName);
					Thread inquiryThread = new Thread(delegate() { InquiryTask(di); });
					inquiryThread.Start();
					System.Threading.Thread.Sleep(500);
				}
			}
		}

        virtual protected void InquiryTask(ModbusDeviceInfo deviceInfo)
        {
            AppendUITextBox("这里是基类的方法呀呀呀!!!!");
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
