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
		#region 全部字段
		private static E_DB_CONNECT_MODE _db_connect_mode = E_DB_CONNECT_MODE.DIRECT;

		public static E_DB_CONNECT_MODE Db_connect_mode
		{
			get { return _db_connect_mode; }
			set { _db_connect_mode = value; }
		}

		// 数据库服务器情报
		protected static ServerInfo _dbServerInfo;

		public static ServerInfo DbServerInfo
		{
			get { return _dbServerInfo; }
			set { _dbServerInfo = value; }
		}

		// 中继服务器情报
		private static ServerInfo _relayServerInfo;

		public static ServerInfo RelayServerInfo
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

		// 查询Timer
		private System.Timers.Timer _inquiryTimer;

		protected System.Timers.Timer InquiryTimer
		{
			get { return _inquiryTimer; }
			set { _inquiryTimer = value; }
		}

		// 断网时, 临时缓存数据用的本地文件
		static string _localBufFileName = "DisconnectBuffer.txt";

		public static string LocalBufFileName
		{
			get { return _localBufFileName; }
		}

		// 用以缓存(因断网)写入DB失败的数据
		static List<string> _bufferList = new List<string>();

		public static List<string> BufferList
		{
			get { return DeviceInquirer._bufferList; }
			set { DeviceInquirer._bufferList = value; }
		}

		private static int _service_area_id;

		public static int Service_area_id
		{
			get { return DeviceInquirer._service_area_id; }
			set { DeviceInquirer._service_area_id = value; }
		}

		// 所在服务区的名称
		static string _service_area_name = string.Empty;

		public static string Service_area_name
		{
			get { return DeviceInquirer._service_area_name; }
			set { DeviceInquirer._service_area_name = value; }
		}

		static Queue<string> _sendBufferQueue = new Queue<string>();

		public static Queue<string> SendBufferQueue
		{
			get { return DeviceInquirer._sendBufferQueue; }
			set { DeviceInquirer._sendBufferQueue = value; }
		}

		static System.Timers.Timer _sendTimer;

		public static System.Timers.Timer SendTimer
		{
			get { return DeviceInquirer._sendTimer; }
			set { DeviceInquirer._sendTimer = value; }
		}

		#endregion

		/// <summary>
		/// 在断网时检查本地缓存文件, 尝试补发缓存的数据
		/// </summary>
		public static void CheckBufferList()
		{
            lock (BufferList)
            {
                // 从本地缓存文件载入缓存数据列表
                LoadLocalBufferList();
                // 尝试重新发送缓存列表中的数据
                ReissueBufferList();
            }
		}

		public static void SendTimerInit()
		{
			SendTimer = new System.Timers.Timer(1000);
			SendTimer.AutoReset = false;
			SendTimer.Elapsed += SendTimer_Elapsed;
			SendTimer.Start();
		}

		static void SendTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			lock (SendBufferQueue)
			{
				int bufferCount = SendBufferQueue.Count;
				if (0 != bufferCount)
				{
					try
					{
                        string dataStr = string.Empty;
						for (int i = 0; i < bufferCount; i++)
						{
                            dataStr = SendBufferQueue.Dequeue();

                            TcpSocketCommunicator reporter = new TcpSocketCommunicator();
                            reporter.Connect(RelayServerInfo.Host_name, RelayServerInfo.Port_num, 5000);
							reporter.Send(Encoding.UTF8.GetBytes(dataStr));
                            reporter.Close();

                            //LogOutput.LogAppend("SendTimer_Elapsed Send : " + dataStr);
							Thread.Sleep(10);
						}
					}
					catch (Exception ex)
					{
						System.Diagnostics.Trace.WriteLine(ex.ToString());
						LogOutput.LogAppend(ex.ToString());
					}
				}
			}

			SendTimer.Start();
		}

		/// <summary>
		/// 从本地缓存文件加载缓存数据
		/// </summary>
        public static void LoadLocalBufferList()
        {
			if (0 != BufferList.Count)
			{
				// 如果不为空, 说明已经load过了, 就不用再重新load了
				return;
			}
			else
			{
				// 否则如果为空, 检查是否存在本地缓存文件, 存在的话load进缓存数据列表里
				if (File.Exists(LocalBufFileName))
				{
					try
					{
						// 读入缓存文件中被缓存的数据
						StreamReader sr = new StreamReader(LocalBufFileName);
						string rdLine = "";
						while (null != (rdLine = sr.ReadLine()))
						{
							if (string.Empty != rdLine.Trim())
							{
								BufferList.Add(rdLine);
							}
						}
						sr.Close();
						File.Delete(LocalBufFileName);
					}
					catch (Exception ex)
					{
						System.Diagnostics.Trace.WriteLine(ex.ToString());
					}
				}
			}
        }

		/// <summary>
		/// 补发缓存数据列表
		/// </summary>
		public static void ReissueBufferList()
		{
			try
			{
                int idx = 0;
                for (idx = 0; idx < BufferList.Count; idx++)
                {
                    if (!WriteToDB(BufferList[idx]))
                    {
                        break;
                    }
                }
				if (0 != idx)
				{
					// 如果有补发成功的要删掉原来的缓存文件
					File.Delete(LocalBufFileName);
                    StreamWriter sw = new StreamWriter(LocalBufFileName, false);
                    List<string> tempList = new List<string>();
                    for (; idx < BufferList.Count; idx++)
                    {
                        tempList.Add(BufferList[idx]);
                        sw.WriteLine(BufferList[idx]);
                    }
                    sw.Close();
					// 清空缓存列表
					BufferList.Clear();
                    BufferList = tempList;
				}
				Thread.Sleep(200);
			}
			catch (Exception ex)
			{
				System.Diagnostics.Trace.WriteLine(ex.ToString());
			}
		}

		/// <summary>
		/// 写入数据库
		/// </summary>
		/// <param name="cmdStr"></param>
		/// <returns></returns>
		protected static bool WriteToDB(string cmdStr)
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
					lock (SendBufferQueue)
					{
						SendBufferQueue.Enqueue(cmdStr);
						//LogOutput.LogAppend("WriteToDB  SendBufferQueue.Enqueue : " + cmdStr);
					}
				}
			}
			catch (Exception ex)
			{
				System.Diagnostics.Trace.WriteLine(ex.ToString());
				LogOutput.LogAppend(ex.ToString());
				return false;
			}
			return true;
		}

		/// <summary>
		/// 上报查询结果给DB服务器
		/// </summary>
		/// <param name="insertStr"></param>
		/// <param name="deviceName"></param>
		/// <returns></returns>
        protected bool ReportToDBServer(string insertStr, string deviceName)
        {
			if (0 != BufferList.Count)
			{
				AppendToBufferList(insertStr);
				AppendUITextBox("	" + deviceName + " : 追加到数据缓存列表!");
				return false;
			}
			else
			{
				if (WriteToDB(insertStr))
				{
					AppendUITextBox("	" + deviceName + " : 数据库保存成功!");
					return true;
				}
				else
				{
					AppendToBufferList(insertStr);
					AppendUITextBox("	" + deviceName + " : 数据库保存失败!");
					return false;
				}
			}
        }

		/// <summary>
		/// 将字串追加到缓存列表的末尾
		/// </summary>
		/// <param name="cmdStr"></param>
		protected void AppendToBufferList(string cmdStr)
		{
			lock (BufferList)
			{
				if (null == BufferList)
				{
					BufferList = new List<string>();
				}
				BufferList.Add(cmdStr);
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
            CheckBufferList();

			InquiryTimer.Start();
			DoInquiry();
		}

		/// <summary>
		/// 查询开始
		/// </summary>
		public void StartInquiry()
		{
            // 0.首先检查有无缓存数据, 有的话要先尝试将缓存数据写入DB
            CheckBufferList();

			// 启动timer
			InquiryTimer = new System.Timers.Timer(CyclePeriod * 60 * 1000);
			InquiryTimer.AutoReset = false;
			InquiryTimer.Elapsed += new System.Timers.ElapsedEventHandler(TimerElapsed);
			InquiryTimer.Start();
			// 开始第一次查询
			DoInquiry();
		}

		public void StopInquiry()
		{
			if (null != InquiryTimer)
			{
				InquiryTimer.Stop();
				_tbxControl = null;
			}
		}

    }
}
