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
    }
}
