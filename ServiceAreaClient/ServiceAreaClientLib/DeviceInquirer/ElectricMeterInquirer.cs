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
		public ElectricMeterInquirer(List<ModbusDeviceInfo> deviceInfoList, ServerInfo sInfo)
        {
            DeviceList = deviceInfoList;
			DbServerInfo = sInfo;
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
                InquiryResult ir = inquirer.Receive();
				AppendUITextBox("	接收到 " + deviceInfo.DeviceName + " 应答数据: " + ir.RcvLen.ToString() + " 字节.");
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
					AppendUITextBox("	" + deviceInfo.DeviceName + " : 数据库连接失败!");
				}
                else
                {
					AppendUITextBox("	" + deviceInfo.DeviceName + " : 数据库写入成功!");
                }

				// TODO: 保存到本地
            }
            catch (Exception ex)
            {
				System.Diagnostics.Trace.WriteLine(ex.ToString());
				AppendUITextBox("	" + deviceInfo.DeviceName + " : 连接失败!");
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
			// 现在只把"正向(吸收)有功电能"即耗电量, 存到数据库表里
			int idx = reportStr.LastIndexOf(',');
			string valueStr = "";
			if (-1 != idx)
			{
				valueStr = reportStr.Substring(idx + 1).Trim();
			}
			int iValue;
			if (!int.TryParse(valueStr, out iValue))
			{
				return false;
			}
			// 读数值要乘以放大倍率
			iValue = iValue * deviceInfo.Magnification;

			// 然后还要除以量纲得到浮点型小数值
			float fValue = iValue / deviceInfo.Magnitude;

            //string argStr = "";
			//for (int i = 0; i < counter; i++)
			//{
			//	argStr += ", value" + (i + 1).ToString().PadLeft(2, '0');
			//}

			// 电表的设备种类编码是001
			string deviceTypeStr = "001";
			// 3位服务区编号 + 3位采集点位置编号 + 3位设备种类编号 + 3位设备地址 = 12位设备编号唯一确定一个具体的设备
			string deviceSnStr = deviceInfo.ServiceArea.ToString().PadLeft(3, '0') + deviceInfo.SpotNumber + deviceTypeStr + deviceInfo.DeviceAddr.ToString().PadLeft(3, '0');
			string insertStr = @"INSERT INTO " + deviceInfo.DbTableName + @"(time_stamp, device_number, value_01" + @") VALUES('" + inquiryResult.TimeStamp + @"'" + @", '"
								+ deviceSnStr + @"'," + fValue.ToString() + @")";
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

    }
}
