using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Net;
using System.Net.Sockets;
using MySql.Data.MySqlClient;
using ServiceAreaClientLib;

namespace UIManager
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            UIInit();
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if ("Start" == btnStart.Text)
            {
                if (!CheckUIValue())
                {
                    return;
                }
                string host = tbxIP1.Text + "." + tbxIP2.Text + "." + tbxIP3.Text + "." + tbxIP4.Text;
                string serverHost = tbxIP5.Text + "." + tbxIP6.Text + "." + tbxIP7.Text + "." + tbxIP8.Text;
                int port, circlePeriod, serverPort;
                int.TryParse(tbxPortNum1.Text, out port);
                int.TryParse(tbxCirclePeriod.Text, out circlePeriod);
                int.TryParse(tbxPortNum2.Text, out serverPort);
                InquiryStart(host, port, circlePeriod, serverHost, serverPort);
                btnStart.Text = "Stop";
                UIEnable(false);
            }
            else
            {
                if (null != _recorder)
                {
                    _recorder.Save2File();
                }
                InquiryStop();
                btnStart.Text = "Start";
                UIEnable(true);
            }
        }

//////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// UI控件初始化
        /// </summary>
        void UIInit()
        {
            tbxIP1.Text = "192";
            tbxIP2.Text = "168";
            tbxIP3.Text = "1";
            tbxIP4.Text = "101";
            tbxPortNum1.Text = "10001";
            tbxCirclePeriod.Text = "10";
            tbxIP5.Text = "114";
            tbxIP6.Text = "215";
            tbxIP7.Text = "104";
            tbxIP8.Text = "146";
            tbxPortNum2.Text = "3306";
        }

        void UIEnable(bool enable)
        {
            tbxIP1.Enabled = enable;
            tbxIP2.Enabled = enable;
            tbxIP3.Enabled = enable;
            tbxIP4.Enabled = enable;
            tbxIP5.Enabled = enable;
            tbxIP6.Enabled = enable;
            tbxIP7.Enabled = enable;
            tbxIP8.Enabled = enable;
            tbxPortNum1.Enabled = enable;
            tbxPortNum2.Enabled = enable;
            tbxCirclePeriod.Enabled = enable;
        }

        /// <summary>
        /// UI参数检查
        /// </summary>
        /// <returns></returns>
        bool CheckUIValue()
        {
            if (    !IPValueCheck(tbxIP1.Text)
                ||  !IPValueCheck(tbxIP2.Text)
                ||  !IPValueCheck(tbxIP3.Text)
                ||  !IPValueCheck(tbxIP4.Text)
                ||  !IPValueCheck(tbxIP5.Text)
                ||  !IPValueCheck(tbxIP6.Text)
                ||  !IPValueCheck(tbxIP7.Text)
                ||  !IPValueCheck(tbxIP8.Text))
            {
                return false;
            }
            int val;
            if (    !int.TryParse(tbxPortNum1.Text, out val)
                ||  !int.TryParse(tbxPortNum2.Text, out val))
            {
                return false;
            }
            if (!int.TryParse(tbxCirclePeriod.Text, out val))
            {
                return false;
            }
            return true;
        }

        bool IPValueCheck(string ipStr)
        {
            int ipVal;
            if (    (string.Empty == ipStr)
                || !int.TryParse(ipStr, out ipVal)  )
            {
                return false;
            }
            if (    ipVal < 0
                ||  ipVal > 255 )
            {
                return false;
            }
            return true;
        }

        TCPCommunicator _inquirer;      // 设备读数查询器
        TCPCommunicator _reporter;      // 向服务器上传数据
        DataRecorder _recorder;         // 数据记录保存

        int _sndRcvCounter;       // 发送/接受回数的计数器
        void InquiryStart(string host, int port, int circlePeriod, string serverHost, int serverPort)
        {
            _inquirer = new TCPCommunicator(host, port);
            _reporter = new TCPCommunicator(serverHost, serverPort);
            _recorder = new DataRecorder();
            try
            {
                textBox1.AppendText("\r\nWeb Server Connect Start...\r\n");
                _reporter.Connect();
                textBox1.AppendText("\r\nWeb Server Connect Success...\r\n");
            }
            catch (Exception ex)
            {
                textBox1.AppendText(ex.ToString());
            }

            try
            {
                textBox1.AppendText("\r\nHDM Connect Start...\r\n");
                _inquirer.Connect();
                textBox1.AppendText("\r\nHDM Connect Success...\r\n");

                timer1.Interval = circlePeriod * 1000;
                _sndRcvCounter = 0;
                SndRcv();
                timer1.Start();
            }
            catch (Exception ex)
            {
                _reporter.Close();
                textBox1.AppendText(ex.ToString());
            }
        }

        string GetBytesStr(byte[] bytesArr, int bytesCnt)
        {
            string retStr = "";
            for (int i = 0; i < bytesCnt; i++)
            {
                retStr += string.Format("{0:X2}", bytesArr[i]);
                if (i != bytesCnt - 1)
                {
                    retStr += " ";
                }
            }
            return retStr;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (    null == _inquirer.Socket
                ||  !_inquirer.Socket.Connected)
            {
                timer1.Stop();
                return;
            }
            SndRcv();
        }

        /// <summary>
        /// 请求送信和接收读数数据
        /// </summary>
        void SndRcv()
        {
            //send message
            byte[] sendBytes = { 0x03, 0x03, 0x00, 0x00, 0x00, 0x4c, 0x45, 0xdd };
            _inquirer.Send(sendBytes);

            //receive message
            string rcvStr = "";
            byte[] rcvBytes = new byte[4096];
            textBox1.AppendText("Receive begin...\r\n");
            int bytes = _inquirer.Receive(rcvBytes, rcvBytes.Length);
            _sndRcvCounter += 1;
            if ((_sndRcvCounter > 1000)
                && (1 == (_sndRcvCounter % 1000)))
            {
                textBox1.Clear();
            }

            rcvStr = GetBytesStr(rcvBytes, bytes);
            string timeStamp = _recorder.Add(rcvStr);

            if (    (null != _reporter)
                &&  _reporter.Socket.Connected  )
            {
                byte[] sndBytes = new byte[bytes];
                for (int i = 0; i < bytes; i++)
                {
                    sndBytes[i] = rcvBytes[i];
                }
                string voltageStr = sndBytes[108].ToString() + sndBytes[109].ToString();    // 电压值
                float voltageVal = Convert.ToInt32(voltageStr, 16)/100;
                string sndStr = timeStamp + "," + voltageVal.ToString();
                // 上传数据
                byte[] sbs = Encoding.ASCII.GetBytes(sndStr);
                _reporter.Send(sbs);
            }
            rcvStr += "\r\n\r\n";
            textBox1.AppendText(_sndRcvCounter.ToString().PadLeft(4, '0') + ": " + bytes.ToString() + " bytes received!\r\n");
            textBox1.AppendText(rcvStr);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (null != _recorder)
            {
                _recorder.Save2File();
            }
            InquiryStop();
        }

        void InquiryStop()
        {
            if (timer1.Enabled)
            {
                timer1.Stop();
            }
            if (null != _inquirer)
            {
                _inquirer.Close();
            }
            if (null != _reporter)
            {
                _reporter.Close();
            }
        }

        private void btnTest_Click(object sender, EventArgs e)
        {
            /*
            DBConnectMySQL mysql_object = new DBConnectMySQL();
//          string insertStr = "INSERT INTO electric_meter VALUES(null,2,3,4,5,6,7,8,9)";
            string deleteStr = "DELETE FROM electric_meter";
            mysql_object.ExecuteMySqlCommand(deleteStr);
             */
            byte[] sendBytes = { 0x03, 0x03, 0x00, 0x00, 0x00, 0x4c, 0x45, 0xdd };
            UInt16 crcVal = ServiceAreaClientLib.ElectricMeterInquirer.CRC16(sendBytes, 6);
            byte crcLow = (byte)(crcVal & 0x00FF);
            byte crcHigh = (byte)((crcVal & 0xFF00) >> 8);
            MessageBox.Show(crcVal.ToString() + crcLow.ToString() + crcHigh.ToString());
        }
    }
}
