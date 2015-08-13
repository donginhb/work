using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;
using MySql.Data.MySqlClient;
using System.Windows.Forms;
using ServiceAreaClientLib.DeviceInquirer;

namespace ServiceAreaClientLib
{
    public class ElectricMeterInquirer
    {
		// 要查询的设备(电表, 水表等)的列表
		List<ModbusDeviceInfo> _deviceList;

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

		internal List<ModbusDeviceInfo> DeviceList
        {
            get { return _deviceList; }
            set { _deviceList = value; }
        }

		public ElectricMeterInquirer(List<ModbusDeviceInfo> deviceInfoList, ServerInfo sInfo)
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
        /// 电表查询执行
        /// </summary>
        public void DoInquiry()
        {
            if (    (null != DeviceList)
                &&  (0 != DeviceList.Count)  )
            {
                AppendUITextBox("\r\n>------------------------------->");
                AppendUITextBox(DateTime.Now.ToLongDateString() + " " + DateTime.Now.ToLongTimeString());
                // 对列表中的各个电表, 逐一进行查询
                for (int i = 0; i < DeviceList.Count; i++)
                {
					ModbusDeviceInfo di = DeviceList[i];
					AppendUITextBox("开始查询 " + di.DeviceSn);
					Thread inquiryThread = new Thread(delegate() { InquiryTask(di); });
					inquiryThread.Start();
                    // 各个设备查询之间要有一定间隔, 否则可能会出现读数不完整或者不同设备读数"读串"的现象
					System.Threading.Thread.Sleep(600);
                }
            }
        }

		/// <summary>
		/// 单个电表查询线程的执行过程
		/// </summary>
		/// <param name="deviceInfo"></param>
		void InquiryTask(ModbusDeviceInfo deviceInfo)
        {
            TcpSocketCommunicator inquirer = new TcpSocketCommunicator();

            try
            {
                // 与设备模块进行连接(Connect)
				// 设定Receive的接收超时时间为3000毫秒
                AppendUITextBox("	开始连接: " + deviceInfo.DeviceSn);
                inquirer.Connect(deviceInfo.HostName, deviceInfo.PortNum, 3000);
                AppendUITextBox("	" + deviceInfo.DeviceSn + "连接成功!");
				System.Threading.Thread.Sleep(100);
                // 向设备模块发送查询指令(Modbus协议)
                //  第一个字节是通信地址(设备号)
                //  第二个字节是功能码0x03(读数据)
                //  后面依次是读的起始地址0x0000和读长度0x004E(=78, 读到"正向(吸收)有功电能")
                //  最后两个字节是CRC16校验码
                byte[] tmpBytes = { (byte)deviceInfo.DeviceAddr, 0x03, 0x00, 0x00, 0x00, 0x4E };

				// 计算CRC校验码
                UInt16 crc16 = CRC16(tmpBytes, 6);
                byte crcLowByte = (byte)(crc16 & 0x00FF);
                byte crcHighByte = (byte)((crc16 & 0xFF00) >> 8);

                byte[] sendBytes = { (byte)deviceInfo.DeviceAddr, 0x03, 0x00, 0x00, 0x00, 0x4E, crcLowByte, crcHighByte };

				// 向设备模块发送Modbus读数查询指令
                AppendUITextBox("	查询 " + deviceInfo.DeviceSn + " 指令发送!");
                inquirer.Send(sendBytes);

                // 接收设备模块返回的读数查询结果
                InquiryResult ir = inquirer.Receive();
                AppendUITextBox("	接收到 " + deviceInfo.DeviceSn + " 应答数据: " + ir.RcvLen.ToString() + " 字节.");
                // 对应答数据进行检查, 返回的第一个字节应该跟设备地址号一致
                if (    (ir.RcvLen >= 1)
                    &&  (deviceInfo.DeviceAddr != ir.RcvBytes[0])    )
                {
                    AppendUITextBox("	" + "收到的应答设备地址不一致: " + ir.RcvBytes[0].ToString());
                    return;
                }

                // 上报给服务器
                if (!Report2Server(ir, deviceInfo))
				{
                    AppendUITextBox("	" + deviceInfo.DeviceSn + " : 数据库连接失败!");
				}
                else
                {
                    AppendUITextBox("	" + deviceInfo.DeviceSn + " : 数据库写入成功!");
                }

				// TODO: 保存到本地
            }
            catch (Exception ex)
            {
				System.Diagnostics.Trace.WriteLine(ex.ToString());
                AppendUITextBox("	" + deviceInfo.DeviceSn + " : 连接失败!");
            }
			finally
			{
				// AppendUITextBox("");
                inquirer.Close();
            }
        }

		bool Report2Server(InquiryResult inquiryResult, ModbusDeviceInfo deviceInfo)
        {
            DBConnectMySQL mysql_object = new DBConnectMySQL(DbServerInfo);
            int counter;
            string reportStr = GetReportString(inquiryResult, out counter);
            string argStr = "";
            for (int i = 0; i < counter; i++)
            {
                argStr += ", value" + (i + 1).ToString().PadLeft(2, '0');
            }
			string deviceSnStr = deviceInfo.ServiceArea.ToString() + deviceInfo.DeviceSn;
			string insertStr = @"INSERT INTO " + deviceInfo.DbTableName + @"(time, device_sn, device_addr" + argStr + @") VALUES('" + inquiryResult.TimeStamp + @"'" + @", "
								+ deviceSnStr + @"," + deviceInfo.DeviceAddr.ToString() + reportStr + @")";
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

        public static string GetReportString(InquiryResult inquiryResult, out int counter)
        {
			string reportStr = "";
			List<DataUnitInfo> dataInfoList = ElectricMeterDataSetting.GetElectricMeterDataSetting();
            counter = 0;
			foreach (var dataInfo in dataInfoList)
			{
                // -5是因为要去掉前面三个字节(分别是设备码， 操作码03， 和读的长度)和最后两个字节(CRC校验)
                if (dataInfo.Offset < inquiryResult.RcvLen - 5 - 1)
				{
					string dataStr = "0x";
                    // 越过前面三个字节
					int idx = dataInfo.Offset + 3;
					for (int i = 0; i < dataInfo.Length; i++)
					{
						dataStr += string.Format("{0:X}", inquiryResult.RcvBytes[idx]).PadLeft(2, '0');
						idx++;
					}
					UInt32 val;
                    try
                    {
                        val = Convert.ToUInt32(dataStr, 16);
                        dataInfo.Value = val;
                        reportStr += ", " + val.ToString();
                        counter++;
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Trace.WriteLine(ex.ToString());
                    }
				}
			}
			return reportStr;
        }

        public static UInt16 CRC16(Byte[] dat, int count)
        {
            uint crc = 0xFFFF;
            int i, j;
            for ( j=0; j<count; j++)
            {
                crc = crc ^ dat[j];
                for ( i=0; i<8; i++)
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
