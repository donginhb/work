using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;
using System.Net;

namespace ServiceAreaClientLib.DeviceInquirer
{
	public class WaterTemperatureInquirer : ModbusDeviceInquirer
	{
		public WaterTemperatureInquirer(List<ModbusDeviceInfo> deviceInfoList, ServerInfo sInfo)
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
		}

		bool Report2Server(string dateTimeStr, string resultStr, ModbusDeviceInfo deviceInfo)
		{
			return true;
		}

	}
}
