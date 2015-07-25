using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServiceAreaClientLib
{
	public class HttpDeviceInfo
	{
		string _name;

		// 设备名称
		public string Name
		{
			get { return _name; }
			set { _name = value; }
		}

		string _requestString;

		// 查询时发送的Http请求字符串
		public string RequestString
		{
			get { return _requestString; }
			set { _requestString = value; }
		}
	}
}
