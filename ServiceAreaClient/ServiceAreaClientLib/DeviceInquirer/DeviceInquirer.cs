using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;
using System.Threading;

namespace ServiceAreaClientLib.DeviceInquirer
{
    public class DeviceInquirer
    {
        private E_DB_CONNECT_MODE _db_connect_mode;

        public E_DB_CONNECT_MODE Db_connect_mode
        {
            get { return _db_connect_mode; }
            set { _db_connect_mode = value; }
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

		// 断网时, 临时缓存数据用的本地文件
        const string _localBufFileName = "DisconnectBuffer.txt";

        public string LocalBufFileName
        {
            get { return _localBufFileName; }
        }

        // 读写本地缓存文件时, 用的lock对象
        protected static object bufferLock = new object();

		// 标记与数据库的连接状态
		private static bool _dataBaseConnected = false;

		protected static bool DataBaseConnected
		{
			get { return DeviceInquirer._dataBaseConnected; }
			set { DeviceInquirer._dataBaseConnected = value; }
		}

        protected void SaveToLocalBuffer(string cmdStr)
        {
            lock (bufferLock)
            {
				try
				{
					StreamWriter sw = new StreamWriter(LocalBufFileName, true);
					sw.WriteLine(cmdStr);
					sw.Close();
				}
				catch (Exception ex)
				{
					System.Diagnostics.Trace.WriteLine(ex.ToString());
				}
            }
        }

		/// <summary>
		/// 在断网时检查本地缓存文件, 尝试补发缓存的数据
		/// </summary>
        protected void CheckLocalBuffer()
        {
			if (DataBaseConnected)
			{
				// 只有在前一次数据库连接为切断状态时才进行处理
				return;
			}
			if (File.Exists(LocalBufFileName))
			{
				lock (bufferLock)
				{
					try
					{
						StreamReader sr = new StreamReader(LocalBufFileName);
						string rdLine = "";
						List<string> cmdList = new List<string>();
						while (null != (rdLine = sr.ReadLine()))
						{
							if (string.Empty != rdLine.Trim())
							{
								cmdList.Add(rdLine);
							}
						}
						sr.Close();
						List<string> failList = new List<string>();
						int idx = 0;
						foreach (string cmd in cmdList)
						{
							if (!WriteToDB(cmd))
							{
								if (0 == idx)
								{
									// 第一个就失败, 表明仍然断网, 网络没有恢复, 接下来没有必要挨个试了
									return;
								}
								failList.Add(cmd);
							}
							idx++;
						}
						// 能走到这说明至少有补发成功的了, 删掉原来的文件, 如果有失败的, 写回本地缓存文件里
						File.Delete(LocalBufFileName);
						if (0 != failList.Count)
						{
							StreamWriter sw = new StreamWriter(LocalBufFileName, true);
							foreach (string cmd in failList)
							{
								sw.WriteLine(cmd);
							}
							sw.Close();
						}
						Thread.Sleep(200);
					}
					catch (Exception ex)
					{
						System.Diagnostics.Trace.WriteLine(ex.ToString());
					}
                }
            }
        }

		protected bool WriteToDB(string cmdStr)
		{
			try
			{
				if (E_DB_CONNECT_MODE.DIRECT == Db_connect_mode)
				{
					// 直接写入数据库
					DBConnectMySQL mysql_object = new DBConnectMySQL(DbServerInfo);
					mysql_object.ExecuteMySqlCommand(cmdStr);
				}
				else if (E_DB_CONNECT_MODE.RELAY == Db_connect_mode)
				{
					// 通过中继服务器
					TcpSocketCommunicator reporter = new TcpSocketCommunicator();
					reporter.Connect(RelayServerInfo.Host_name, RelayServerInfo.Port_num, 5000);
					reporter.Send(Encoding.ASCII.GetBytes(cmdStr));
					reporter.Close();
				}
			}
			catch (Exception ex)
			{
				System.Diagnostics.Trace.WriteLine(ex.ToString());
				return false;
			}
			return true;
		}

        protected bool Report2Server(string insertStr, string deviceName)
        {
            if (WriteToDB(insertStr))
			{
				// 数据库保存成功
				lock (this)
				{
					DataBaseConnected = true;
				}
				AppendUITextBox("	" + deviceName + " : 数据库保存成功!");
			}
            else
            {
				// 数据库保存失败
				lock (this)
				{
					DataBaseConnected = false;
				}
				AppendUITextBox("	" + deviceName + " : 数据库保存失败!");
				Thread th = new Thread(delegate() { SaveToLocalBuffer(insertStr); });
				th.Start();
				return false;
            }
			return true;
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

		virtual public void DoInquiry()
		{
			AppendUITextBox("这可是基类的虚方法呀呀呀!!!!");
		}

		protected void TimerElapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			_timer.Start();
			DoInquiry();
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
				_tbxControl = null;
			}
		}

    }
}
