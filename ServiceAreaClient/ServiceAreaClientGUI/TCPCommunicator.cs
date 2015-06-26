using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;

namespace UIManager
{
    /// <summary>
    /// 设备读数查询器
    /// </summary>
    class TCPCommunicator
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

        public TCPCommunicator(string hostName, int portNum)
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

        public int Receive(byte[] rcvBytes, int len)
        {
            int bytes = _socket.Receive(rcvBytes, len, 0);
            return bytes;
        }

        public void Close()
        {
            if (null != _socket
                && _socket.Connected)
            {
                _socket.Close();
            }
        }
    }
}
