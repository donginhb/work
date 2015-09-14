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

        protected void SaveToLocalFile(string cmdStr)
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

        protected void CheckLocalBufferFile()
        {
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
						File.Delete(LocalBufFileName);
						List<string> failList = new List<string>();
						foreach (string cmd in cmdList)
						{
							if (!Report2Server(cmd, string.Empty))
							{
								failList.Add(cmd);
							}
						}
						// 失败的话, 再存回本地缓存文件
						if (0 != failList.Count)
						{
							StreamWriter sw = new StreamWriter(LocalBufFileName, true);
							foreach (string cmd in failList)
							{
								sw.WriteLine(cmd);
							}
							sw.Close();
						}
					}
					catch (Exception ex)
					{
						System.Diagnostics.Trace.WriteLine(ex.ToString());
					}
                }
            }
        }

        protected bool Report2Server(string insertStr, string deviceName)
        {
			// 用于数据库写入成功/失败后续处理的子线程
			Thread th;
            try
            {
                if (E_DB_CONNECT_MODE.DIRECT == Db_connect_mode)
                {
                    // 直接写入数据库
                    DBConnectMySQL mysql_object = new DBConnectMySQL(DbServerInfo);
                    mysql_object.ExecuteMySqlCommand(insertStr);
                }
                else if (E_DB_CONNECT_MODE.RELAY == Db_connect_mode)
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
				// 数据库保存失败
				if (string.Empty != deviceName)
				{
					AppendUITextBox("	" + deviceName + " : 数据库保存失败!");
					th = new Thread(delegate() { SaveToLocalFile(insertStr); });
					th.Start();
				}
				else
				{
					// 如果是[string.Empty], 说明是补发缓存的数据
					// 这时如果失败在外层调用处一并写回缓存文件(不在这里写)
					AppendUITextBox("	缓存数据补发失败 : " + insertStr);
				}
				return false;
            }
            finally
            {
                ;
            }
			// 数据库保存成功
			if (string.Empty != deviceName)
			{
				AppendUITextBox("	" + deviceName + " : 数据库保存成功!");
				th = new Thread(delegate() { CheckLocalBufferFile(); });
				th.Start();
			}
			else
			{
				// 如果是[string.Empty], 说明是补发缓存的数据
				AppendUITextBox("	缓存数据补发成功 : " + insertStr);
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

    }
}
