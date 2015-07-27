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
			ModbusDeviceInquirer modbusInqurier = null;
			HttpDeviceInquirer httpInqurier = null;
            if ("Start" == btnStart.Text)
            {
                if (!CheckUIValue())
                {
                    return;
                }
                string serverHost = tbxIP1.Text + "." + tbxIP2.Text + "." + tbxIP3.Text + "." + tbxIP4.Text;
                int updatePeriod, serverPort;
                int.TryParse(tbxUpdatePeriod.Text, out updatePeriod);
                int.TryParse(tbxPortNum.Text, out serverPort);
				ServerInfo sInfo = new ServerInfo(serverHost, serverPort, tbxDBName.Text, tbxUsrName.Text, tbxPassword.Text);
                btnStart.Text = "Stop";
                UIEnable(false);

				// 1.生成查询设备列表
				List<ModbusDeviceInfo> modbusList;
				List<HttpDeviceInfo> httpList;
				CreateInquiryDeviceList(out modbusList, out httpList);

				// 2.查询开始
				modbusInqurier = ModbusInquiryStart(modbusList, sInfo);
				httpInqurier = HttpInquiryStart(httpList);
            }
            else
            {
				// 停止查询
				ModbusInquiryStop(modbusInqurier);
				HttpInquiryStop(httpInqurier);
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
			//tbxIP1.Text = "114";
			//tbxIP2.Text = "215";
			//tbxIP3.Text = "104";
			//tbxIP4.Text = "146";
			//tbxPortNum.Text = "3306";
			//tbxDBName.Text = "service_area";
			//tbxUsrName.Text = "sarea";
			//tbxPassword.Text = "Huachang2015!";
			tbxIP1.Text = "127";
			tbxIP2.Text = "0";
			tbxIP3.Text = "0";
			tbxIP4.Text = "1";
			tbxPortNum.Text = "3306";
			tbxDBName.Text = "saem_db";
			tbxUsrName.Text = "admin";
			tbxPassword.Text = "admin";
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
			item.SubItems.Add(@"http://192.168.0.79/nvc-cgi/admin/vca.cgi?action=list&group=VCA.Ch0.Ct0.count");
			item.Checked = true;
			listView2.Items.Add(item);

			item = new ListViewItem("摄像头1, 计数器1");
			item.SubItems.Add(@"http://192.168.0.79/nvc-cgi/admin/vca.cgi?action=list&group=VCA.Ch0.Ct1.count");
			item.Checked = true;
			listView2.Items.Add(item);

			tbxUpdatePeriod.Text = "1";
        }

        void UIEnable(bool enable)
        {
            tbxIP1.Enabled = enable;
            tbxIP2.Enabled = enable;
            tbxIP3.Enabled = enable;
            tbxIP4.Enabled = enable;
			tbxPortNum.Enabled = enable;
			tbxUpdatePeriod.Enabled = enable;
        }

        /// <summary>
        /// UI参数检查
        /// </summary>
        /// <returns></returns>
        bool CheckUIValue()
        {
            if (
				!IPValueCheck(tbxIP1.Text)
                ||  !IPValueCheck(tbxIP2.Text)
                ||  !IPValueCheck(tbxIP3.Text)
                ||  !IPValueCheck(tbxIP4.Text))
            {
                return false;
            }
            int val;
            if (!int.TryParse(tbxPortNum.Text, out val))
            {
                return false;
            }
			if (!int.TryParse(tbxUpdatePeriod.Text, out val))
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

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
        }

		/// <summary>
		/// 测试用
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
        private void btnTest_Click(object sender, EventArgs e)
        {
			ServerInfo sInfo = new ServerInfo("127.0.0.1", 3306, "saem_db", "admin", "admin");
			DBConnectMySQL mysql_object = new DBConnectMySQL(sInfo);
			string insertStr = "INSERT INTO electric_meter VALUES(null,2,3)";
//			string deleteStr = "DELETE FROM electric_meter";
			mysql_object.ExecuteMySqlCommand(insertStr);

			//byte[] sendBytes = { 0x03, 0x03, 0x00, 0x00, 0x00, 0x4c, 0x45, 0xdd };
			//InquiryResult ir = new InquiryResult();
			//ir.RcvBytes = sendBytes;
			//ir.RcvLen = 8;
			//string str = ModbusDeviceInquirer.GetReportString(ir);
			//MessageBox.Show(str);

			//WebClient wc = new WebClient();
			//string resultStr = wc.DownloadString(new Uri(@"http://192.168.0.79/nvc-cgi/admin/vca.cgi?action=list&group=VCA.Ch1.Ct0.count"));
			//MessageBox.Show(resultStr);
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
		private void CreateInquiryDeviceList(out List<ModbusDeviceInfo> modbusList, out List<HttpDeviceInfo> httpList)
		{
			modbusList = new List<ModbusDeviceInfo>();
			// 遍历ListView控件取得各个查询设备的参数情报
			// 首先是Modbus设备
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
				deviceInfo.TableName = "electric_meter";

				// 加入到查询设备列表中
				modbusList.Add(deviceInfo);
			}

			httpList = new List<HttpDeviceInfo>();
			// 然后是Http设备(摄像头)
			foreach (ListViewItem item in listView2.Items)
			{
				if (!item.Checked)
				{
					continue;
				}
				HttpDeviceInfo deviceInfo = new HttpDeviceInfo();
				string[] paraArr = new string[10];
				int idx = 0;
				foreach (ListViewItem.ListViewSubItem subitems in item.SubItems)
				{
					paraArr[idx] = subitems.Text.Trim();
					idx++;
				}
				// 设备名称
				deviceInfo.Name = paraArr[0];
				// Request String
				deviceInfo.RequestString = paraArr[1];

				httpList.Add(deviceInfo);
			}
		}

		/// <summary>
		/// 开始查询
		/// </summary>
		private ModbusDeviceInquirer ModbusInquiryStart(List<ModbusDeviceInfo> modbusList, ServerInfo sInfo)
		{
			ModbusDeviceInquirer inquirer = new ModbusDeviceInquirer(modbusList, sInfo);
			int value;
			if (int.TryParse(tbxUpdatePeriod.Text, out value))
			{
				inquirer.CyclePeriod = value;
			}
			inquirer.TbxControl = textBox1;
			inquirer.StartInquiry();

			return inquirer;
		}

		void ModbusInquiryStop(ModbusDeviceInquirer inqurier)
		{
			if (null != inqurier)
			{
				inqurier.StopInquiry();
				inqurier = null;
			}
		}

		private HttpDeviceInquirer HttpInquiryStart(List<HttpDeviceInfo> httpList)
		{
			HttpDeviceInquirer inquirer = new HttpDeviceInquirer(httpList);
			int value;
			if (int.TryParse(tbxUpdatePeriod.Text, out value))
			{
				inquirer.CyclePeriod = value;
			}
			inquirer.TbxControl = textBox1;
			inquirer.StartInquiry();

			return inquirer;
		}

		void HttpInquiryStop(HttpDeviceInquirer inqurier)
		{
			if (null != inqurier)
			{
				inqurier.StopInquiry();
				inqurier = null;
			}
		}

    }
}
