using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using ServiceAreaClientLib;
using System.Net;

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
			ModbusDeviceInquirer inqurier = null;
            if ("Start" == btnStart.Text)
            {
                if (!CheckUIValue())
                {
                    return;
                }
//              string host = tbxIP1.Text + "." + tbxIP2.Text + "." + tbxIP3.Text + "." + tbxIP4.Text;
                string serverHost = tbxIP5.Text + "." + tbxIP6.Text + "." + tbxIP7.Text + "." + tbxIP8.Text;
                int port, circlePeriod, serverPort;
//              int.TryParse(tbxPortNum1.Text, out port);
//              int.TryParse(tbxCirclePeriod.Text, out circlePeriod);
                int.TryParse(tbxPortNum2.Text, out serverPort);
//              InquiryStart(host, port, circlePeriod, serverHost, serverPort);
                btnStart.Text = "Stop";
                UIEnable(false);

				// 1.生成查询设备列表
				List<ModbusDeviceInfo> iqList = CreateInquiryDeviceList();

				// 2.查询开始
				inqurier = InquiryStart(iqList);
            }
            else
            {
				InquiryStop(inqurier);
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
//          tbxIP1.Text = "192";
//          tbxIP2.Text = "168";
//          tbxIP3.Text = "1";
//          tbxIP4.Text = "101";
//          tbxPortNum1.Text = "10001";
//          tbxCirclePeriod.Text = "10";
            tbxIP5.Text = "114";
            tbxIP6.Text = "215";
            tbxIP7.Text = "104";
            tbxIP8.Text = "146";
            tbxPortNum2.Text = "3306";
			tbxDBName.Text = "service_area";
			tbxUsrName.Text = "sarea";
			tbxPassword.Text = "Huachang2015!";
			cbxPassword.Checked = true;

			// Modbus设备列表
			listView1.Columns.Add("Name", 90);
			listView1.Columns.Add("Device");
			listView1.Columns.Add("IP", 80);
			listView1.Columns.Add("Port");
			listView1.Columns.Add("Addr");
			listView1.Columns.Add("Offset");

			ListViewItem item = new ListViewItem("电表1");
			item.SubItems.Add("1");
			item.SubItems.Add("192.168.0.7");
			item.SubItems.Add("23");
			item.SubItems.Add("0");
			item.SubItems.Add("76");
			item.Checked = true;
			listView1.Items.Add(item);

			item = new ListViewItem("电表2");
			item.SubItems.Add("2");
			item.SubItems.Add("192.168.0.7");
			item.SubItems.Add("23");
			item.SubItems.Add("0");
			item.SubItems.Add("76");
			item.Checked = true;
			listView1.Items.Add(item);

			item = new ListViewItem("电表3");
			item.SubItems.Add("3");
			item.SubItems.Add("192.168.0.7");
			item.SubItems.Add("23");
			item.SubItems.Add("0");
			item.SubItems.Add("76");
			item.Checked = true;
			listView1.Items.Add(item);

			// Http设备列表
			listView2.Columns.Add("Name", 130);
			listView2.Columns.Add("Request string", 550);
			item = new ListViewItem("摄像头1, 计数器0");
			item.SubItems.Add(@"http://192.168.26.79/nvc-cgi/admin/vca.cgi?action=list&group=VCA.Ch0.Ct0.count");
			item.Checked = true;
			listView2.Items.Add(item);

			item = new ListViewItem("摄像头1, 计数器1");
			item.SubItems.Add(@"http://192.168.26.79/nvc-cgi/admin/vca.cgi?action=list&group=VCA.Ch0.Ct1.count");
			item.Checked = true;
			listView2.Items.Add(item);

			tbxUpdatePeriod.Text = "1";
        }

        void UIEnable(bool enable)
        {
//          tbxIP1.Enabled = enable;
//          tbxIP2.Enabled = enable;
//          tbxIP3.Enabled = enable;
//          tbxIP4.Enabled = enable;
            tbxIP5.Enabled = enable;
            tbxIP6.Enabled = enable;
            tbxIP7.Enabled = enable;
            tbxIP8.Enabled = enable;
//          tbxPortNum1.Enabled = enable;
//          tbxPortNum2.Enabled = enable;
//          tbxCirclePeriod.Enabled = enable;
        }

        /// <summary>
        /// UI参数检查
        /// </summary>
        /// <returns></returns>
        bool CheckUIValue()
        {
            if (
				/*
				!IPValueCheck(tbxIP1.Text)
                ||  !IPValueCheck(tbxIP2.Text)
                ||  !IPValueCheck(tbxIP3.Text)
                ||  !IPValueCheck(tbxIP4.Text)
                || */
				!IPValueCheck(tbxIP5.Text)
                ||  !IPValueCheck(tbxIP6.Text)
                ||  !IPValueCheck(tbxIP7.Text)
                ||  !IPValueCheck(tbxIP8.Text))
            {
                return false;
            }
            int val;
            if (!int.TryParse(tbxPortNum2.Text, out val))
            {
                return false;
            }
			//if (!int.TryParse(tbxCirclePeriod.Text, out val))
			//{
			//	return false;
			//}
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
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
        }

        private void btnTest_Click(object sender, EventArgs e)
        {
            /*
            DBConnectMySQL mysql_object = new DBConnectMySQL();
//          string insertStr = "INSERT INTO electric_meter VALUES(null,2,3,4,5,6,7,8,9)";
            string deleteStr = "DELETE FROM electric_meter";
            mysql_object.ExecuteMySqlCommand(deleteStr);
             */

			//byte[] sendBytes = { 0x03, 0x03, 0x00, 0x00, 0x00, 0x4c, 0x45, 0xdd };
			//InquiryResult ir = new InquiryResult();
			//ir.RcvBytes = sendBytes;
			//ir.RcvLen = 8;
			//string str = ModbusDeviceInquirer.GetReportString(ir);
			//MessageBox.Show(str);

			WebClient wc = new WebClient();
			string resultStr = wc.DownloadString(new Uri(@"http://192.168.26.79/nvc-cgi/admin/vca.cgi?action=list&group=VCA.Ch0.Ct0.count"));
			MessageBox.Show(resultStr);
        }

		private void btnAdd1_Click(object sender, EventArgs e)
		{

		}

		private void btnDel1_Click(object sender, EventArgs e)
		{

		}

		private void btnEdit1_Click(object sender, EventArgs e)
		{

		}

		/// <summary>
		/// 生成查询设备列表
		/// </summary>
		private List<ModbusDeviceInfo> CreateInquiryDeviceList()
		{
			List<ModbusDeviceInfo> inquiryDeviceList = new List<ModbusDeviceInfo>();
			// 遍历ListView控件取得各个查询设备的参数情报
			foreach (ListViewItem item in listView1.Items)
			{
				if (!item.Checked)
				{
					continue;
				}
				ModbusDeviceInfo deviceInfo = new ModbusDeviceInfo();
				string[] paraArr = new string[10];
				int idx = 0;
				foreach (ListViewItem.ListViewSubItem subitems in item.SubItems)
				{
					paraArr[idx] = subitems.Text.Trim();
					idx++;
				}
				// 设备名称
				deviceInfo.DeviceName = paraArr[0];
				// 设备编号
				int value;
				if (int.TryParse(paraArr[1], out value))
				{
					deviceInfo.DeviceNum = value;
				}
				// Host IP
				deviceInfo.HostName = paraArr[2];
				// 端口号
				if (int.TryParse(paraArr[3], out value))
				{
					deviceInfo.PortNum = value;
				}
				// 读数据起始地址
				if (int.TryParse(paraArr[4], out value))
				{
					deviceInfo.ReadAddr = value;
				}
				// 读数据长度
				if (int.TryParse(paraArr[5], out value))
				{
					deviceInfo.ReadLength = value;
				}

				// 加入到查询设备列表中
				inquiryDeviceList.Add(deviceInfo);
			}

			return inquiryDeviceList;
		}

		/// <summary>
		/// 开始查询
		/// </summary>
		private ModbusDeviceInquirer InquiryStart(List<ModbusDeviceInfo> inquiryList)
		{
			ModbusDeviceInquirer inquirer = new ModbusDeviceInquirer(inquiryList);
			int value;
			if (int.TryParse(tbxUpdatePeriod.Text, out value))
			{
				inquirer.CyclePeriod = value;
			}
			inquirer.TbxControl = textBox1;
			inquirer.StartInquiry();

			return inquirer;
		}

		void InquiryStop(ModbusDeviceInquirer inqurier)
		{
			if (null != inqurier)
			{
				inqurier.StopInquiry();
				inqurier = null;
			}
		}

    }
}
