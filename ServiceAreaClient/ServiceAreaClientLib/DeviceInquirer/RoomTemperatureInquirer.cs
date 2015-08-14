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
		public RoomTemperatureInquirer(List<ModbusDeviceInfo> deviceInfoList, ServerInfo sInfo)
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
				InquiryResult ir = inquirer.Receive();
				AppendUITextBox("	接收到 " + deviceInfo.DeviceName + " 应答数据: " + ir.RcvLen.ToString() + " 字节.");
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
				AppendUITextBox("	" + deviceInfo.DeviceName + " 返回值: " + temperatureStr);
				// 上报给服务器
                Report2Server(dateTimeStr, temperatureStr, deviceInfo);

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

		bool Report2Server(string dateTimeStr, string temperatureStr, ModbusDeviceInfo deviceInfo)
		{
            DBConnectMySQL mysql_object = new DBConnectMySQL(DbServerInfo);
            float temperatureVal = 0;
            if (!float.TryParse(temperatureStr, out temperatureVal))
            {
                return false;
            }
            string reportStr = ", " + temperatureVal.ToString();
            string deviceSnStr = deviceInfo.ServiceArea.ToString() + deviceInfo.SpotNumber;
            string insertStr = @"INSERT INTO " + deviceInfo.DbTableName + @"(time, device_sn, device_addr, value01" + @") VALUES('"
									+ dateTimeStr + @"'" + @"," + deviceSnStr + @", " + deviceInfo.DeviceAddr.ToString() + reportStr + @")";
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
