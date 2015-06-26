using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;

namespace ServiceAreaClientLib
{
    class TcpSocketCommunicator
    {
        string _host = "";
        int _port = -1;
        Socket _socket;

        public Socket Socket
        {
            get { return _socket; }
            set { _socket = value; }
        }

        public int Port
        {
            get { return _port; }
            set { _port = value; }
        }

        public string Host
        {
            get { return _host; }
            set { _host = value; }
        }

        public TcpSocketCommunicator(string hostName, int portNum)
        {
            Host = hostName;
            Port = portNum;
        }

        public void Connect()
        {
            IPAddress ip = IPAddress.Parse(_host);
            IPEndPoint ipe = new IPEndPoint(ip, _port);

            _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            _socket.Connect(ipe);
        }

        public void Send(byte[] sendBytes)
        {
            _socket.Send(sendBytes);
        }

        public InquiryResult Receive()
        {
            InquiryResult ir = new InquiryResult();
            byte[] rcvBytes = new byte[1024];
            int bytes = _socket.Receive(rcvBytes, rcvBytes.Length, 0);
            ir.RcvBytes = rcvBytes;
            ir.TimeStamp = GetTimeStamp();
            return ir;
        }

        public void Close()
        {
            if (null != _socket
                && _socket.Connected)
            {
                _socket.Close();
            }
        }

        public long GetTimeStamp()
        {
            TimeSpan ts = DateTime.Now.ToUniversalTime() - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            return Convert.ToInt64(ts.TotalSeconds);
        }
    }
}
