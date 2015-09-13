using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;

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

        const string _disconnectBufferFileName = "DisconnectBuffer.txt";

        public string DisconnectBufferFileName
        {
            get { return _disconnectBufferFileName; }
        }

        // 读写本地缓存文件时, 用的lock对象
        protected static object bufferLock = new object();

        protected void SaveToLocalFile(string cmdStr)
        {
            lock (bufferLock)
            {
                StreamWriter sw = new StreamWriter(DisconnectBufferFileName, true);
                sw.WriteLine(cmdStr);
                sw.Close();
            }
        }

        protected void CheckLocalBufferFile()
        {
            lock (bufferLock)
            {
                if (File.Exists(DisconnectBufferFileName))
                {
                    StreamReader sr = new StreamReader(DisconnectBufferFileName);
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
                    File.Delete(DisconnectBufferFileName);
                    List<string> failList = new List<string>();
                    foreach (string cmd in cmdList)
                    {
                        if (!Report2Server(cmd))
                        {
                            failList.Add(cmd);
                        }
                    }
                    // 失败的话, 再存回本地缓存文件
                    if (0 != failList.Count)
                    {
                        StreamWriter sw = new StreamWriter(DisconnectBufferFileName, true);
                        foreach (string cmd in failList)
                        {
                            sw.WriteLine(cmd);
                        }
                        sw.Close();
                    }
                }
            }
        }

        protected bool Report2Server(string insertStr)
        {
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
                return false;
            }
            finally
            {
                ;
            }
            return true;
        }

    }
}
