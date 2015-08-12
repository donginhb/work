using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServiceAreaClientLib
{
    /// <summary>
    /// 电表情报
    /// </summary>
    public class ElectricMeterInfo
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

		string _deviceSn;

		public string DeviceSn		 // 设备编号(区域编号3位 + 设备种别编号3位 + 具体设备编号3位, 共九位, 插入数据库时与前面的三位服务区编号一起构成12位的完整设备序号)
		{
			get { return _deviceSn; }
			set { _deviceSn = value; }
		}

        int _deviceAddr;             // 设备地址

        public int DeviceAddr
        {
            get { return _deviceAddr; }
            set { _deviceAddr = value; }
        }

        string _hostName;           // ip字符串

        public string HostName
        {
            get { return _hostName; }
            set { _hostName = value; }
        }

        int _portNum;               // 访问端口号

        public int PortNum
        {
            get { return _portNum; }
            set { _portNum = value; }
        }

		int _readAddr;				// 读操作的起始地址

		public int ReadAddr
		{
			get { return _readAddr; }
			set { _readAddr = value; }
		}
		int _readLength;			// 读操作的长度

		public int ReadLength
		{
			get { return _readLength; }
			set { _readLength = value; }
		}

		string _tableName;			// 数据库中的表名

		public string TableName
		{
			get { return _tableName; }
			set { _tableName = value; }
		}

		public int _dataSetting;
    }
}
