using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServiceAreaClientLib
{
	public class PassengerCounterInfo
	{
		int _serviceArea;			// 服务区编号

		public int ServiceArea
		{
			get { return _serviceArea; }
			set { _serviceArea = value; }
		}

		string _deviceName;			// 设备名称

		public string DeviceName
		{
			get { return _deviceName; }
			set { _deviceName = value; }
		}

		string _spotNumber;

		public string SpotNumber		 // 区域(采集点)位置编号
		{
			get { return _spotNumber; }
			set { _spotNumber = value; }
		}

		string _requestString1;

		// 查询时发送的Http请求字符串
		public string RequestString1
		{
			get { return _requestString1; }
			set { _requestString1 = value; }
		}

		string _requestString2;

		public string RequestString2
		{
			get { return _requestString2; }
			set { _requestString2 = value; }
		}


		string _dbTableName;

		public string DbTableName
		{
			get { return _dbTableName; }
			set { _dbTableName = value; }
		}
	}
}
