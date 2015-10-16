using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServiceAreaClientLib.DeviceInquirer
{
	public class ModbusDeviceInfo
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

		int _deviceAddr;

		// 设备地址
		public int DeviceAddr
		{
			get { return _deviceAddr; }
			set { _deviceAddr = value; }
		}

		string _dbTableName;

		public string DbTableName
		{
			get { return _dbTableName; }
			set { _dbTableName = value; }
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

		public int _dataSetting;

		// 读数放大倍率, 即结果由读数的实际值乘以这个倍率得出, 默认值是1
		private float _magnification = 1;

		public float Magnification
		{
			get { return _magnification; }
			set { _magnification = value; }
		}

		// 读数的量纲, 即由整数读数, 除以10的整数倍, 得到实际的小数值.
		int _magnitude = 1;

		public int Magnitude
		{
			get { return _magnitude; }
			set { _magnitude = value; }
		}

        // 校正值
        float _adjustment = 0;

        public float Adjustment
        {
            get { return _adjustment; }
            set { _adjustment = value; }
        }

		// 分组编号: 用以横向对比时, 区分不同的分组
		// 默认值(无效值): 0
		// 电表: 1:开水机, 2:锅炉房, 3:高杆灯, 4:公厕
		// 水表: 1:开水机, 2:锅炉房, 3:公厕
		int _groupId = 0;

		public int GroupId
		{
			get { return _groupId; }
			set { _groupId = value; }
		}
	}
}
