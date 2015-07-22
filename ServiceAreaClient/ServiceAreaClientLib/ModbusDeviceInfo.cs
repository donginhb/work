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
    public class ModbusDeviceInfo
    {
		string _deviceName;			// 设备名称

		public string DeviceName
		{
			get { return _deviceName; }
			set { _deviceName = value; }
		}

        int _deviceNum;             // 设备编号(地址编号)

        public int DeviceNum
        {
            get { return _deviceNum; }
            set { _deviceNum = value; }
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
    }
}
