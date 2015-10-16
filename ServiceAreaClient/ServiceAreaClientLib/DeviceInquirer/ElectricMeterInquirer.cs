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
	public class ElectricMeterInquirer : ModbusDeviceInquirer
    {
		public ElectricMeterInquirer(	List<ModbusDeviceInfo> deviceInfoList, ServerInfo dbServer,
										ServerInfo relayServer, E_DB_CONNECT_MODE dbConnectMode)
        {
            DeviceList = deviceInfoList;
			DbServerInfo = dbServer;
			RelayServerInfo = relayServer;
			Db_connect_mode = dbConnectMode;
        }

		/// <summary>
		/// 单个电表查询线程的执行过程
		/// </summary>
		/// <param name="deviceInfo"></param>
		override protected void InquiryTask(ModbusDeviceInfo deviceInfo)
        {
            TcpSocketCommunicator inquirer = new TcpSocketCommunicator();
            try
            {
                // 与设备模块进行连接(Connect)
				// 设定Receive的接收超时时间为3000毫秒
                AppendUITextBox("	开始连接: " + deviceInfo.DeviceName);
                inquirer.Connect(deviceInfo.HostName, deviceInfo.PortNum, 3000);
				AppendUITextBox("	" + deviceInfo.DeviceName + "连接成功!");
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
				AppendUITextBox("	查询 " + deviceInfo.DeviceName + " 指令发送!");
                inquirer.Send(sendBytes);

                // 接收设备模块返回的读数查询结果
                ReceiveData rd = inquirer.Receive();
				AppendUITextBox("	接收到 " + deviceInfo.DeviceName + " 应答数据: " + rd.RcvLen.ToString() + " 字节.");
                // 对应答数据进行检查, 返回的第一个字节应该跟设备地址号一致
                if (    (rd.RcvLen >= 1)
                    &&  (deviceInfo.DeviceAddr != rd.RcvBytes[0])    )
                {
                    AppendUITextBox("	" + "收到的应答设备地址不一致: " + rd.RcvBytes[0].ToString());
                    return;
                }

				float fValue;
				string insertStr = GetReportString(rd, deviceInfo, out fValue);
				if (null == insertStr)
				{
					return;
				}
                AppendUITextBox("	" + deviceInfo.DeviceName + " : 读数值 = "
					+ fValue.ToString() + deviceInfo.Adjustment.ToString() + " = "
					+ (fValue + deviceInfo.Adjustment).ToString());
				// 上报给服务器
				Report2Server(insertStr, deviceInfo.DeviceName);
            }
            catch (Exception ex)
            {
				System.Diagnostics.Trace.WriteLine(ex.ToString());
				AppendUITextBox("	" + deviceInfo.DeviceName + " : 查询失败!");
            }
			finally
			{
                inquirer.Close();
            }
        }

        public static string GetReportString(ReceiveData inquiryResult, ModbusDeviceInfo deviceInfo, out float fValue)
        {
			string reportStr = "";
			fValue = 0;
			List<DataUnitInfo> dataInfoList = ElectricMeterDataSetting.GetElectricMeterDataSetting();
			int total_flg = 0;	// 数据库表中用以标识该电表是否为"总表"的标志, 如果是总表, 那么不在表示用电量占比的饼图和柱状图中显示其用电量;
			if (-1 != deviceInfo.DeviceName.IndexOf(@"总表"))
			{
				total_flg = 1;
			}

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
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Trace.WriteLine(ex.ToString());
                    }
				}
			}

			// 现在只把"正向(吸收)有功电能"即耗电量, 存到数据库表里
			string[] arr = reportStr.Split(',');
			string valueStr = arr[arr.Length - 4].Trim();
			int iValue;
			if (!int.TryParse(valueStr, out iValue))
			{
				return null;
			}
			// 读数值要乘以放大倍率
			float value = iValue * deviceInfo.Magnification;

			// 然后还要除以量纲得到浮点型小数值
			fValue = value / deviceInfo.Magnitude;
			// 加上校正值进行校正调整得到最终的用电量
			float realVal = fValue + deviceInfo.Adjustment;

			// 电表的设备种类编码是001
			string deviceTypeStr = "001";
			// 3位服务区编号 + 3位采集点位置编号 + 3位设备种类编号 + 3位设备地址 = 12位设备编号唯一确定一个具体的设备
			string deviceSnStr = deviceInfo.ServiceArea.ToString().PadLeft(3, '0')
								+ deviceInfo.SpotNumber.PadLeft(3, '0') + deviceTypeStr
								+ deviceInfo.DeviceAddr.ToString().PadLeft(3, '0');

			string insertStr = @"INSERT INTO " + deviceInfo.DbTableName + @"(time_stamp, device_number, value_01, total_flg, group_id" + @") VALUES('" + inquiryResult.TimeStamp + @"'" + @", '"
								+ deviceSnStr + @"'," + realVal.ToString() + ", " + total_flg.ToString() + ", " + deviceInfo.GroupId.ToString() + @")";

			return insertStr;
		}

    }
}
