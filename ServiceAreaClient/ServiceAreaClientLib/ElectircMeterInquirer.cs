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
        public ElectricMeterInquirer()
        {
            _eMeterList = GetMeterList();
        }

        /// <summary>
        /// 电表查询执行
        /// </summary>
        public List<InquiryResult> DoRun()
        {
            List<InquiryResult> resultList = new List<InquiryResult>();

            // 对列表中的各个电表, 逐一进行查询
            for (int i = 0; i < _eMeterList.Count; i++)
            {
                InquiryResult rslt = Inquiry(_eMeterList[i]);
                resultList.Add(rslt);
            }

            return resultList;
        }

        /// <summary>
        /// 取得电表情报的列表
        /// </summary>
        /// <returns></returns>
        List<ElectricMeterInfo> GetMeterList()
        {
            List<ElectricMeterInfo> mList = new List<ElectricMeterInfo>();

            return mList;
        }

        InquiryResult Inquiry(ElectricMeterInfo meterInfo)
        {
            //send message
            byte[] sendBytes = { 0x03, 0x03, 0x00, 0x00, 0x00, 0x4c, 0x45, 0xdd };
            TcpSocketCommunicator inquirer = new TcpSocketCommunicator(meterInfo.HostName, meterInfo.PortNum);
            inquirer.Send(sendBytes);

            //receive message
            InquiryResult ir = inquirer.Receive();

            return ir;
        }

        public UInt16 GetCRC16(byte[] bytes, int len)
        {
            UInt16 crc16 = 0xFFFF;
            for (int i = 0; i < len; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    
                }
            }

            return crc16;
        }
    }
}
