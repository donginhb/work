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
	public struct DbCmd
	{
		public string cmdStr;
		public string deviceName;

		public DbCmd(string cmd, string device)
		{
			cmdStr = cmd;
			deviceName = device;
		}
	}

	public class ModbusDeviceInquirer
	{
		// 要查询的设备列表
		protected List<ModbusDeviceInfo> _deviceList;

		public List<ModbusDeviceInfo> DeviceList
		{
			get { return _deviceList; }
			set { _deviceList = value; }
		}

		// 数据库服务器情报
		protected ServerInfo _dbServerInfo;

		public ServerInfo DbServerInfo
		{
			get { return _dbServerInfo; }
			set { _dbServerInfo = value; }
		}

		// 中继服务器情报
		private ServerInfo _relayServerInfo;

		protected ServerInfo RelayServerInfo
		{
			get { return _relayServerInfo; }
			set { _relayServerInfo = value; }
		}

		private E_DB_CONNECT_MODE _db_connect_mode;

		public E_DB_CONNECT_MODE Db_connect_mode
		{
			get { return _db_connect_mode; }
			set { _db_connect_mode = value; }
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

		// 数据库(插入)命令列表
		List<DbCmd> _dbCmdList;

		public List<DbCmd> DbCmdList
		{
			get { return _dbCmdList; }
			set { _dbCmdList = value; }
		}

		const string _disconnectBufferFileName = "DisconnectBuffer.txt";

		public string DisconnectBufferFileName
		{
			get { return _disconnectBufferFileName; }
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
				Task[] taskArr = new Task[DeviceList.Count];
				for (int i = 0; i < DeviceList.Count; i++)
				{
					ModbusDeviceInfo di = DeviceList[i];
					AppendUITextBox("开始查询 " + di.DeviceName);
					// Thread inquiryThread = new Thread(delegate() { InquiryTask(di); });
					taskArr[i] = new Task(delegate() { InquiryTask(di); });
					// inquiryThread.Start();
					taskArr[i].Start();
					System.Threading.Thread.Sleep(500);
				}
				Task.WaitAll(taskArr);
				ReportToDataBase();
			}
		}

		protected void ReportToDataBase()
		{
			try
			{
				if (E_DB_CONNECT_MODE.DIRECT == Db_connect_mode)
				{
					// 直接写入数据库
					DBConnectMySQL mysql_object = new DBConnectMySQL(DbServerInfo);
					foreach (DbCmd cmd in DbCmdList)
					{
						mysql_object.ExecuteMySqlCommand(cmd.cmdStr);
						DbCmdList.Remove(cmd);
					}
				}
				else if (E_DB_CONNECT_MODE.RELAY == Db_connect_mode)
				{
					// 通过中继服务器
					TcpSocketCommunicator reporter = new TcpSocketCommunicator();
					reporter.Connect(RelayServerInfo.Host_name, RelayServerInfo.Port_num, 5000);
					foreach (DbCmd cmd in DbCmdList)
					{
						reporter.Send(Encoding.ASCII.GetBytes(cmd.cmdStr));
					}
					reporter.Close();
				}
			}
			catch (Exception ex)
			{
				System.Diagnostics.Trace.WriteLine(ex.ToString());
			}
			finally
			{
				if (0 != DbCmdList.Count)
				{
					// 说明有未成功写入数据库的项
					foreach (DbCmd cmd in DbCmdList)
					{
						AppendUITextBox("	" + cmd.deviceName + " : 数据库保存失败!");
					}
					// 这时要缓存到本地文件里, 等以后连上数据库以后再上传这部分数据
					SaveToLocalFile(DbCmdList);
				}
			}
		}

		protected void SaveToLocalFile(List<DbCmd> cmdList)
		{
			StreamWriter sw = new StreamWriter(DisconnectBufferFileName, true);
			foreach (DbCmd cmd in cmdList)
			{
				sw.WriteLine(cmd.cmdStr);
			}
			sw.Close();
		}

		protected bool Report2Server(string insertStr, ModbusDeviceInfo deviceInfo)
		{
			try
			{
				if (E_DB_CONNECT_MODE.DIRECT == Db_connect_mode)
				{
					// 直接写入数据库
					DBConnectMySQL mysql_object = new DBConnectMySQL(DbServerInfo);
					mysql_object.ExecuteMySqlCommand(insertStr);
				}
				else
				{
					// 通过中继服务器
					TcpSocketCommunicator reporter = new TcpSocketCommunicator();
					reporter.Connect(RelayServerInfo.Host_name, RelayServerInfo.Port_num, 5000);
					reporter.Send(Encoding.ASCII.GetBytes(insertStr));
					reporter.Close();
				}
			}
			catch (Exception ex)
			{
				System.Diagnostics.Trace.WriteLine(ex.ToString());
				return false;
			}
			finally
			{
				;
			}
			return true;
		}

        virtual protected void InquiryTask(ModbusDeviceInfo deviceInfo)
        {
            AppendUITextBox("这里是基类的方法呀呀呀!!!!");
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
                if (_tbxControl.Text.Length >= _tbxControl.MaxLength - 1000)
                {
                    _tbxControl.Text = _tbxControl.Text.Substring(_tbxControl.Text.Length - 100);
                }
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
