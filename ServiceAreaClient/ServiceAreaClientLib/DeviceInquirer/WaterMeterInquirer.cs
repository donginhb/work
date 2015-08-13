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
		void InquiryTask(ModbusDeviceInfo deviceInfo)
		{
			try
			{
				string dateTimeStr = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");

				TcpSocketCommunicator inquirer = new TcpSocketCommunicator();

				// 与设备模块进行连接(Connect)
				// 设定Receive的接收超时时间为3000毫秒
				AppendUITextBox("	开始连接: " + deviceInfo.DeviceSn);
				inquirer.Connect(deviceInfo.HostName, deviceInfo.PortNum, 3000);
				AppendUITextBox("	" + deviceInfo.DeviceSn + "连接成功!");
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
				byte[] sendBytes = { 0x08, 0x03, 0x02, 0x02, 0x00, 0x02, crcLowByte, crcHighByte };

				// 向设备模块发送读数查询指令
				AppendUITextBox("	查询 " + deviceInfo.DeviceSn + " 指令发送!");
				inquirer.Send(sendBytes);

				// 接收设备模块返回的读数查询结果
				InquiryResult ir = inquirer.Receive();
				AppendUITextBox("	接收到 " + deviceInfo.DeviceSn + " 应答数据: " + ir.RcvLen.ToString() + " 字节.");
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
					inquirer.Close();
					return;
				}
				AppendUITextBox("	" + deviceInfo.DeviceSn + " 返回值: " + temperatureStr);
				// 上报给服务器
				Report2Server(dateTimeStr, temperatureStr, deviceInfo);

				// 保存到本地

				inquirer.Close();
			}
			catch (Exception ex)
			{
				AppendUITextBox("	" + deviceInfo.DeviceSn + ": 查询失败!");
				System.Diagnostics.Trace.WriteLine(ex.ToString());
			}
		}

		bool Report2Server(string dateTimeStr, string resultStr, ModbusDeviceInfo deviceInfo)
		{
			return true;
		}

	}
}
