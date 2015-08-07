using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServiceAreaClientLib
{
	public class HttpDeviceInfo
	{
		int _serviceArea;			// 服务区编号

		public int ServiceArea
		{
			get { return _serviceArea; }
			set { _serviceArea = value; }
		}

		string _name;				// 设备名称

		public string Name
		{
			get { return _name; }
			set { _name = value; }
		}

		string _deviceSn;

		public string DeviceSn		 // 设备编号(区域编号3位 + 设备种别编号3位 + 具体设备编号3位, 共九位, 插入数据库时与前面的三位服务区编号一起构成12位的完整设备序号)
		{
			get { return _deviceSn; }
			set { _deviceSn = value; }
		}

		string _requestString;

		// 查询时发送的Http请求字符串
		public string RequestString
		{
			get { return _requestString; }
			set { _requestString = value; }
		}

		string _dbTableName;

		public string DbTableName
		{
			get { return _dbTableName; }
			set { _dbTableName = value; }
		}
	}
}
