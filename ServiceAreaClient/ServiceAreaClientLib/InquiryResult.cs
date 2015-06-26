using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServiceAreaClientLib
{
    public class InquiryResult
    {
        long _timeStamp;

        public long TimeStamp                // 时间戳
        {
            get { return _timeStamp; }
            set { _timeStamp = value; }
        }

        byte[] _rcvBytes;
        int _rcvLen;

        public int RcvLen
        {
            get { return _rcvLen; }
            set { _rcvLen = value; }
        }

        public byte[] RcvBytes              // 查询结果
        {
            get { return _rcvBytes; }
            set { _rcvBytes = value; }
        }
    }
}
