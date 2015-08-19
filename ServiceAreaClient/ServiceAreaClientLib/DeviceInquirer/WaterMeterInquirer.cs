using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;
using System.Net;

namespace ServiceAreaClientLib.DeviceInquirer
{
	public class WaterMeterInquirer : ModbusDeviceInquirer
	{
		public WaterMeterInquirer(List<ModbusDeviceInfo> deviceInfoList, ServerInfo sInfo)
        {
            DeviceList = deviceInfoList;
			DbServerInfo = sInfo;
        }

		/// <summary>
		/// 单个设备查询线程的执行过程
		/// </summary>
		/// <param name="deviceInfo"></param>
        override protected void InquiryTask(ModbusDeviceInfo deviceInfo)
		{
			string dateTimeStr = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
			TcpSocketCommunicator inquirer = new TcpSocketCommunicator();
			try
			{
				// 与设备模块进行连接(Connect)
				// 设定Receive的接收超时时间为3000毫秒
				AppendUITextBox("	开始连接: " + deviceInfo.DeviceName);
				inquirer.Connect(deviceInfo.HostName, deviceInfo.PortNum, 3000);
				AppendUITextBox("	" + deviceInfo.DeviceName + "连接成功!");
				System.Threading.Thread.Sleep(100);

				// 发送查询命令内容
				// 目标设备地址
				// 功能码: 0x03(读)
				// 寄存器地址: 0x0202
				// 读长度: 0x0002
				byte[] tmpBytes = { (byte)deviceInfo.DeviceAddr, 0x03, 0x02, 0x02, 0x00, 0x02 };

				UInt16 crc16 = CRC16(tmpBytes, 6);
				byte crcLowByte = (byte)(crc16 & 0x00FF);
				byte crcHighByte = (byte)((crc16 & 0xFF00) >> 8);
				// 水表
				byte[] sendBytes = { (byte)deviceInfo.DeviceAddr, 0x03, 0x02, 0x02, 0x00, 0x02, crcLowByte, crcHighByte };

				// 向设备模块发送读数查询指令
				AppendUITextBox("	查询 " + deviceInfo.DeviceName + " 指令发送!");
				inquirer.Send(sendBytes);

				// 接收设备模块返回的读数查询结果
				InquiryResult ir = inquirer.Receive();
				AppendUITextBox("	接收到 " + deviceInfo.DeviceName + " 应答数据: " + ir.RcvLen.ToString() + " 字节.");
				if ((ir.RcvLen >= 1)
					&& (deviceInfo.DeviceAddr != ir.RcvBytes[0]))
				{
					AppendUITextBox("	" + "收到的应答设备地址不一致: " + ir.RcvBytes[0].ToString());
					return;
				}
				if (ir.RcvLen < 3)
				{
					System.Diagnostics.Trace.WriteLine(@"收到数据不正确, 无长度位!");
					return;
				}
				int data_len = ir.RcvBytes[2];
				if (ir.RcvLen < data_len + 3)
				{
					System.Diagnostics.Trace.WriteLine(@"收到数据长度不正确!");
					return;
				}
				string waterVolumeStr = "";
				for (int i = 3; i < 3 + data_len; i++)
				{
					string valStr = string.Format("{0:X}", ir.RcvBytes[i]).PadLeft(2, '0');
					waterVolumeStr += valStr;
				}
				int iWaterVolumeVal = Convert.ToInt32(waterVolumeStr, 16);
				// 首先读数要乘以放大倍率
				float waterVolumeVal = iWaterVolumeVal * deviceInfo.Magnification;
				// 然后除以量纲得到实际小数值
				float fWaterVolumeVal = waterVolumeVal / deviceInfo.Magnitude;

				AppendUITextBox("	" + deviceInfo.DeviceName + " 返回值: " + fWaterVolumeVal.ToString());
				// 上报给服务器
				if (!Report2Server(dateTimeStr, fWaterVolumeVal, deviceInfo))
                {
                    AppendUITextBox("	" + deviceInfo.DeviceName + " : 数据库写入失败!");
                }

				// TODO:保存到本地
			}
			catch (Exception ex)
			{
				AppendUITextBox("	" + deviceInfo.DeviceName + ": 查询失败!");
				System.Diagnostics.Trace.WriteLine(ex.ToString());
			}
			finally
			{
				inquirer.Close();
			}
		}

		bool Report2Server(string dateTimeStr, float waterVolumeVal, ModbusDeviceInfo deviceInfo)
		{
			DBConnectMySQL mysql_object = new DBConnectMySQL(DbServerInfo);
			string reportStr = waterVolumeVal.ToString();
			// 水表的设备种类编码是002
			string deviceTypeStr = "002";
			string deviceSnStr = deviceInfo.ServiceArea.ToString().PadLeft(3, '0') + deviceInfo.SpotNumber.PadLeft(3, '0') + deviceTypeStr + deviceInfo.DeviceAddr.ToString().PadLeft(3, '0');
			string insertStr = @"INSERT INTO " + deviceInfo.DbTableName + @"(time_stamp, device_number, value_01" + @") VALUES('"
									+ dateTimeStr + @"'," + deviceSnStr + @", " + reportStr + @")";
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

	}
}
