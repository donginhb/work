using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;
using System.Net;
using ServiceAreaClientLib.DeviceInquirer;

namespace ServiceAreaClientLib
{
	public class RoomTemperatureInquirer : ModbusDeviceInquirer
	{
		public RoomTemperatureInquirer(	List<ModbusDeviceInfo> deviceInfoList)
        {
            DeviceList = deviceInfoList;
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
				// 帧头:0x10
				// 总字节数: 0x07
				// 目标地址: 4byte16进制数
				// IO功能编号: 0x0A PT100温度采集
				string deviceAddrStr = deviceInfo.DeviceAddr.ToString().PadLeft(8, '0');
				if (8 != deviceAddrStr.Length)
				{
					return;
				}
				byte b1 = Convert.ToByte(deviceAddrStr.Substring(0, 2), 16);
				byte b2 = Convert.ToByte(deviceAddrStr.Substring(2, 2), 16);
				byte b3 = Convert.ToByte(deviceAddrStr.Substring(4, 2), 16);
				byte b4 = Convert.ToByte(deviceAddrStr.Substring(6, 2), 16);
				
				byte[] sendBytes = { 0x10, 0x07, b1, b2, b3, b4, 0x0a };

				// 向设备模块发送读数查询指令
				AppendUITextBox("	查询 " + deviceInfo.DeviceName + " 指令发送!");
				inquirer.Send(sendBytes);

				// 接收设备模块返回的读数查询结果
				ReceiveData ir = inquirer.Receive();
				AppendUITextBox("	接收到 " + deviceInfo.DeviceName + " 应答数据: " + ir.RcvLen.ToString() + " 字节.");
				// 室温返回的结果是字符串, 直接是以小数表示, 所以不用除以量纲
                string outStr = System.Text.Encoding.ASCII.GetString(ir.RcvBytes).Substring(0, ir.RcvLen);
                int idx = -1;
                string temperatureStr = "";
                // 加号表示零上温度，减号表示零下温度
                if (-1 != (idx = outStr.IndexOf('+')))
                {
                    temperatureStr = outStr.Substring(idx).Trim();
                }
                else if (-1 != (idx = outStr.IndexOf('-')))
                {
                    temperatureStr = outStr.Substring(idx).Trim();
                }
                else
                {
                    return;
                }
                float temperatureVal = 0;
                if (!float.TryParse(temperatureStr, out temperatureVal))
                {
                    return;
                }
                // 加上校正值进行校正调整得到最终的温度值
                float fValue = temperatureVal + deviceInfo.Adjustment;

				AppendUITextBox("	" + deviceInfo.DeviceName + " 返回值: " + temperatureStr + " + ("
									+ deviceInfo.Adjustment.ToString() + ") = " + fValue.ToString());
				// 上报给服务器
				string insertStr = GetReportString(dateTimeStr, fValue, deviceInfo);
                AppendUITextBox("	" + deviceInfo.DeviceName + " : 读数值 = " + fValue.ToString());
				ReportToDBServer(insertStr, deviceInfo.DeviceName);

				// 判断温度值是否在正常区间内
				int alarmType = 0;
				if (0 != (alarmType = IsTemperatureOutOfRange(fValue)))
				{
					AddTemperatureAlarmRecord(deviceInfo, alarmType, fValue);
				}
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

		public static string GetReportString(string dateTimeStr, float temperatureVal, ModbusDeviceInfo deviceInfo)
		{
			string reportStr = temperatureVal.ToString();
			// 室温的设备种类编码是004
			string deviceTypeStr = "004";
			string deviceSnStr = deviceInfo.ServiceArea.ToString().PadLeft(3, '0') + deviceInfo.SpotNumber.PadLeft(3, '0') + deviceTypeStr + deviceInfo.DeviceAddr.ToString().PadLeft(3, '0');
			string insertStr = @"INSERT INTO " + deviceInfo.DbTableName + @"(time_stamp, device_number, value_01" + @") VALUES('"
									+ dateTimeStr + @"'," + deviceSnStr + @", " + reportStr + @")";

			return insertStr;
		}

		/// <summary>
		/// 判断室温温度值是否在规定的范围内
		/// </summary>
		/// <param name="temperatureVal"></param>
		/// <returns></returns>
		int IsTemperatureOutOfRange(float temperatureVal)
		{
			float TemperatureValMin = 12;
			float TemperatureValMax = 28;
			// 根据季节, 时间段, 确定室温的上下限
			DateTime now = DateTime.Now;
			int year = DateTime.Now.Year;
			int month = DateTime.Now.Month;
			int day = DateTime.Now.Day;
			int hour = DateTime.Now.Hour;
			int minute = DateTime.Now.Minute;
			int second = DateTime.Now.Second;
			if (	month == 6
				||	month == 7
				||	month == 8)
			{
				// 夏季, 以下三个时间段: 6时至8时; 11时至14时; 16时至20时;
				DateTime dt11 = new DateTime(year, month, day, 6, 0, 0);
				DateTime dt12 = new DateTime(year, month, day, 8, 0, 0);
				DateTime dt21 = new DateTime(year, month, day, 11, 0, 0);
				DateTime dt22 = new DateTime(year, month, day, 14, 0, 0);
				DateTime dt31 = new DateTime(year, month, day, 16, 0, 0);
				DateTime dt32 = new DateTime(year, month, day, 20, 0, 0);
				if (
						(now >= dt11 && now <= dt12)
					||	(now >= dt21 && now <= dt22)
					||	(now >= dt31 && now <= dt32)
					)
				{
				// 最高温度不高于26度
					TemperatureValMax = 26;
				}
				else
				{
				// 其它时间段不高于28度
					TemperatureValMax = 28;
				}
			}
			else if (	month == 12
					 ||	month == 1
					 ||	month == 2)
			{
				// 冬季, 以下四个时间段:
				DateTime dt11 = new DateTime(year, month, day,  6,  0, 0);		// 6时至8时30分
				DateTime dt12 = new DateTime(year, month, day,  8, 30, 0);
				DateTime dt21 = new DateTime(year, month, day, 11, 30, 0);		// 11时30到14时
				DateTime dt22 = new DateTime(year, month, day, 14,  0, 0);
				DateTime dt31 = new DateTime(year, month, day, 17,  0, 0);		// 17时至19时30分
				DateTime dt32 = new DateTime(year, month, day, 19, 30, 0);
				DateTime dt41 = new DateTime(year, month, day, 21,  0, 0);		// 21时至23时30分
				DateTime dt42 = new DateTime(year, month, day, 23, 30, 0);
				// 最低温度不低于16度
				if (
						(now >= dt11 && now <= dt12)
					||	(now >= dt21 && now <= dt22)
					||	(now >= dt31 && now <= dt32)
					||	(now >= dt41 && now <= dt42)
					)
				{
					TemperatureValMin = 16;
				}
				else
				{
					TemperatureValMin = 12;
				}
			}
			else
			{
				// 其它(春秋)
				TemperatureValMax = 28;
				TemperatureValMin = 12;
			}

			if (temperatureVal < TemperatureValMin)
			{
				return -1;
			}
			else if (temperatureVal > TemperatureValMax)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

		/// <summary>
		/// 追加室温异常警报记录
		/// </summary>
		/// <param name="deviceInfo"></param>
		void AddTemperatureAlarmRecord(ModbusDeviceInfo deviceInfo, int alarmType, float alarmValue)
		{
			// date_time
			string dateTimeStr = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
			// sarea_id
			string sareaIdStr = deviceInfo.ServiceArea.ToString().PadLeft(3, '0');
			// spot_id
			string spotIdStr = deviceInfo.SpotNumber.ToString().PadLeft(3, '0');
			// device_type 室温固定是"004"
			string deviceTypeStr = "004";
			// device_id
			string deviceIdStr = deviceInfo.DeviceAddr.ToString().PadLeft(3, '0');
			string alarmTypeStr = string.Empty;
			if (alarmType > 0)
			{
				alarmTypeStr = @"高于上限";
			}
			else
			{
				alarmTypeStr = @"低于下限";
			}
			// alart_message
			string alarmMsgStr = Service_area_name + " " + deviceInfo.DeviceName + " 室温异常 : " + alarmTypeStr;
			// value_01 = alarmValue

			string insertStr = @"INSERT INTO " + "temperature_alarm_record"
								+ @"(date_time, sarea_id, spot_id, device_type, device_id, alarm_message, value_01"
								+ @") VALUES('" + dateTimeStr + @"','" + sareaIdStr + @"', '" + spotIdStr + @"', '"
								+ deviceTypeStr + @"', '" + deviceIdStr + @"', '" + alarmMsgStr + @"', " + alarmValue.ToString() + @")";
			WriteToDB(insertStr);
			if (!SaveToLocalFile(insertStr))
			{
				AppendUITextBox("	" + deviceInfo.DeviceName + " 室温异常" + " : 保存本地记录文件失败!");
			}
		}
	}
}
