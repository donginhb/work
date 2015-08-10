using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;
using System.Net;

namespace ServiceAreaClientLib
{
	public class ZigbeeDeviceInquirer
	{
		// 要查询的设备列表
        List<ZigbeeDeviceInfo> _deviceList;

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

        internal List<ZigbeeDeviceInfo> DeviceList
        {
            get { return _deviceList; }
            set { _deviceList = value; }
        }

		public ZigbeeDeviceInquirer(List<ZigbeeDeviceInfo> deviceInfoList, ServerInfo sInfo)
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
			if ((null != DeviceList)
				&& (0 != DeviceList.Count))
			{
				AppendUITextBox("\r\n>------------------------------->");
				AppendUITextBox(DateTime.Now.ToLongDateString() + " " + DateTime.Now.ToLongTimeString());
				// 对列表中的各个设备, 逐一进行查询
				for (int i = 0; i < DeviceList.Count; i++)
				{
					ZigbeeDeviceInfo di = DeviceList[i];
					AppendUITextBox("开始查询 " + di.DeviceSn);
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
		void InquiryTask(ZigbeeDeviceInfo deviceInfo)
		{
			try
			{
				TcpSocketCommunicator inquirer = new TcpSocketCommunicator();

				// 与设备模块进行连接(Connect)
				// 设定Receive的接收超时时间为3000毫秒
				AppendUITextBox("	开始连接: " + deviceInfo.DeviceSn);
				inquirer.Connect(deviceInfo.HostName, deviceInfo.PortNum, 3000);
				AppendUITextBox("	" + deviceInfo.DeviceSn + "连接成功!");
				System.Threading.Thread.Sleep(100);

				// 发送查询命令内容
				// 帧头:0x10
				// 总字节数: 0x07
				// 目标地址: 4byte16进制数
				// IO功能编号: 0x0A PT100温度采集
				if (8 != deviceInfo.DeviceAddr.PadLeft(8, '0').Length)
				{
					return;
				}
				byte b1 = Convert.ToByte(deviceInfo.DeviceAddr.Substring(0, 2), 16);
				byte b2 = Convert.ToByte(deviceInfo.DeviceAddr.Substring(2, 2), 16);
				byte b3 = Convert.ToByte(deviceInfo.DeviceAddr.Substring(4, 2), 16);
				byte b4 = Convert.ToByte(deviceInfo.DeviceAddr.Substring(6, 2), 16);
				
				byte[] sendBytes = { 0x10, 0x07, b1, b2, b3, b4, 0x0a };

				// 向设备模块发送读数查询指令
				AppendUITextBox("	查询 " + deviceInfo.DeviceSn + " 指令发送!");
				inquirer.Send(sendBytes);

				// 接收设备模块返回的读数查询结果
				InquiryResult ir = inquirer.Receive();
				AppendUITextBox("	接收到 " + deviceInfo.DeviceSn + " 应答数据: " + ir.RcvLen.ToString() + " 字节.");

				// 上报给服务器
				if (!Report2Server(ir, deviceInfo))
				{
					AppendUITextBox("	" + deviceInfo.DeviceSn + " : 数据库连接失败!");
				}
				else
				{
					AppendUITextBox("	" + deviceInfo.DeviceSn + " : 数据库写入成功!");
				}

				// 保存到本地

				inquirer.Close();
			}
			catch (Exception ex)
			{
				AppendUITextBox("	" + deviceInfo.DeviceSn + ": 查询失败!");
				System.Diagnostics.Trace.WriteLine(ex.ToString());
			}
		}

		bool Report2Server(InquiryResult inquiryResult, ZigbeeDeviceInfo deviceInfo)
		{
			return true;
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
