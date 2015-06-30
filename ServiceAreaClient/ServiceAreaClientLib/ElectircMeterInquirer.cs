using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServiceAreaClientLib
{
    public class ElectricMeterInquirer
    {
        List<ElectricMeterInfo> _eMeterList;

        internal List<ElectricMeterInfo> EMeterList
        {
            get { return _eMeterList; }
            set { _eMeterList = value; }
        }

        public ElectricMeterInquirer(List<ElectricMeterInfo> meterInfoList)
        {
            EMeterList = meterInfoList;
        }

        /// <summary>
        /// 电表查询执行
        /// </summary>
        public List<InquiryResult> DoRun()
        {
            List<InquiryResult> resultList = new List<InquiryResult>();
            if (null != _eMeterList)
            {
                // 对列表中的各个电表, 逐一进行查询
                for (int i = 0; i < _eMeterList.Count; i++)
                {
                    InquiryResult rslt = Inquiry(_eMeterList[i]);
                    resultList.Add(rslt);
                }
            }
            return resultList;
        }

        InquiryResult Inquiry(ElectricMeterInfo meterInfo)
        {
            // send message
            // 第一个字节是通信地址(设备号)
            // 第二个字节是功能码0x03(读数据)
            // 后面依次是读的起始地址0x0000和读长度0x004C
            // 最后两个字节是CRC16校验码
            byte[] tmpBytes = { (byte)meterInfo.DeviceNum, 0x03, 0x00, 0x00, 0x00, 0x4C };

            UInt16 crc16 = CRC16(tmpBytes, 6);
            byte crcLow = (byte)(crc16 & 0x00FF);
            byte crcHigh = (byte)((crc16 & 0xFF00) >> 8);

            byte[] sendBytes = { (byte)meterInfo.DeviceNum, 0x03, 0x00, 0x00, 0x00, 0x4C, crcLow, crcHigh };
            TcpSocketCommunicator inquirer = new TcpSocketCommunicator(meterInfo.HostName, meterInfo.PortNum);
            inquirer.Send(sendBytes);

            //receive message
            InquiryResult ir = inquirer.Receive();

            return ir;
        }

        public static UInt16 CRC16(Byte[] dat, int count)
        {
            uint crc = 0xFFFF;
            int i, j;
            for ( j=0; j<count; j++)
            {
                crc = crc ^ dat[j];
                for ( i=0; i<8; i++)
                {
                    if ((crc & 0x0001) > 0)
                    {
                        crc = crc >> 1;
                        crc = crc ^ 0xa001;
                    }
                    else
                    {
                        crc = crc >> 1;
                    }
                }
            }
            return (UInt16)(crc);
        }

    }
}
