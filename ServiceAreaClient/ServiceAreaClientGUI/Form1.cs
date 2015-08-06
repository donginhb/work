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

namespace ServiceAreaClient
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            UIInit();
        }

        ModbusDeviceInquirer _modbusInqurier = null;

        public ModbusDeviceInquirer ModbusInqurier
        {
            get { return _modbusInqurier; }
            set { _modbusInqurier = value; }
        }

        HttpDeviceInquirer _httpInqurier = null;

        public HttpDeviceInquirer HttpInqurier
        {
            get { return _httpInqurier; }
            set { _httpInqurier = value; }
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
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
				ModbusInqurier = ModbusInquiryStart(modbusList, sInfo);
				HttpInqurier = HttpInquiryStart(httpList, sInfo);

				SaveIniFile();
            }
            else
            {
				// 停止查询
				ModbusInquiryStop(ModbusInqurier);
				HttpInquiryStop(HttpInqurier);
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
			// 读取INI文件
			LoadIniFile();

			// 读取XML文件初始化ListView
			List<ListView> ctrlList = new List<ListView>();
			ctrlList.Add(listView1);
			ctrlList.Add(listView2);
			XmlFile.LoadListViewItems(ctrlList);
        }

        void UIEnable(bool enable)
        {
            // 文本框
            tbxIP1.Enabled = enable;
            tbxIP2.Enabled = enable;
            tbxIP3.Enabled = enable;
            tbxIP4.Enabled = enable;
			tbxPortNum.Enabled = enable;
			tbxUpdatePeriod.Enabled = enable;
			tbxServiceAreaNum.Enabled = enable;
			tbxDBName.Enabled = enable;
			tbxUsrName.Enabled = enable;
			tbxPassword.Enabled = enable;

            // 列表
			listView1.Enabled = enable;
			listView2.Enabled = enable;

            // 按钮
            btnAdd1.Enabled = enable;
            btnDel1.Enabled = enable;
            btnEdit1.Enabled = enable;
            btnAdd2.Enabled = enable;
            btnDel2.Enabled = enable;
            btnEdit2.Enabled = enable;
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
			SaveIniFile();
			List<ListView> ctrlList = new List<ListView>();
			ctrlList.Add(listView1);
			ctrlList.Add(listView2);
			XmlFile.SaveListViewItems(ctrlList);
        }

		/// <summary>
		/// 测试用
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
        private void btnTest_Click(object sender, EventArgs e)
        {
			//ServerInfo sInfo = new ServerInfo("127.0.0.1", 3306, "saem_db", "admin", "admin");
			//string dtStr = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
			//DBConnectMySQL mysql_object = new DBConnectMySQL(sInfo);
			//string insertStr = @"INSERT INTO electric_meter (time) VALUES('" + dtStr + @"')";
//			string deleteStr = "DELETE FROM electric_meter";
			//try
			//{
			//	mysql_object.ExecuteMySqlCommand(insertStr);
			//}
			//catch (Exception ex)
			//{
			//	MessageBox.Show(ex.ToString());
			//}

			//byte[] sendBytes = { 0x03, 0x03, 0x00, 0x00, 0x00, 0x4c, 0x45, 0xdd };
			//InquiryResult ir = new InquiryResult();
			//ir.RcvBytes = sendBytes;
			//ir.RcvLen = 8;
			//string str = ModbusDeviceInquirer.GetReportString(ir);
			//MessageBox.Show(str);

			//WebClient wc = new WebClient();
			//string resultStr = wc.DownloadString(new Uri(@"http://192.168.0.79/nvc-cgi/admin/vca.cgi?action=list&group=VCA.Ch1.Ct0.count"));
			//MessageBox.Show(resultStr);

			List<ListView> ctrlList = new List<ListView>();
			ctrlList.Add(listView1);
			ctrlList.Add(listView2);
			XmlFile.SaveListViewItems(ctrlList);

			XmlFile.LoadListViewItems(ctrlList);
        }

		private void btnAdd1_Click(object sender, EventArgs e)
		{
			ButtonAddClick(listView1);
		}

		private void btnDel1_Click(object sender, EventArgs e)
		{
			ButtonDelClick(listView1);
		}

		private void btnEdit1_Click(object sender, EventArgs e)
		{
			ButtonEditClick(listView1);
		}

		private void btnAdd2_Click(object sender, EventArgs e)
		{
			ButtonAddClick(listView2);
		}

		private void btnDel2_Click(object sender, EventArgs e)
		{
			ButtonDelClick(listView2);
		}

		private void btnEdit2_Click(object sender, EventArgs e)
		{
			ButtonEditClick(listView2);
		}

		void ButtonAddClick(ListView list_view_ctrl)
		{
			DeviceEditForm editForm = new DeviceEditForm(list_view_ctrl.Columns.Count);
			if (DialogResult.OK == editForm.ShowDialog())
			{
				ListViewItem item = new ListViewItem(editForm.ParaList[0]);
				for (int i = 1; i < editForm.ParaList.Count; i++)
				{
					item.SubItems.Add(editForm.ParaList[i]);
				}
				list_view_ctrl.Items.Add(item);
			}
		}

		void ButtonDelClick(ListView list_view_ctrl)
		{
			foreach (ListViewItem item in list_view_ctrl.SelectedItems)
			{
				list_view_ctrl.Items.Remove(item);
			}
		}

		void ButtonEditClick(ListView list_view_ctrl)
		{
			if (0 == list_view_ctrl.SelectedItems.Count)
			{
				return;
			}
			ListViewItem selectedItem = list_view_ctrl.SelectedItems[0];
			List<string> pList = new List<string>();
			foreach (ListViewItem.ListViewSubItem item in selectedItem.SubItems)
			{
				pList.Add(item.Text);
			}

			DeviceEditForm editForm = new DeviceEditForm(list_view_ctrl.Columns.Count, pList);
			if (DialogResult.OK == editForm.ShowDialog())
			{
				if (0 == editForm.ParaList.Count)
				{
					return;
				}
				selectedItem.SubItems.Clear();
				selectedItem.Text = editForm.ParaList[0];
				for (int i = 1; i < editForm.ParaList.Count; i++)
				{
					selectedItem.SubItems.Add(editForm.ParaList[i]);
				}
			}
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
				string[] paraArr = new string[item.SubItems.Count];
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
				deviceInfo.TableName = paraArr[6];

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
				string[] paraArr = new string[item.SubItems.Count];
				int idx = 0;
				foreach (ListViewItem.ListViewSubItem subitems in item.SubItems)
				{
					paraArr[idx] = subitems.Text.Trim();
					idx++;
				}
				// 设备名称
				deviceInfo.Name = paraArr[0];
				deviceInfo.DbTableName = paraArr[1];
				// Request String
				deviceInfo.RequestString = paraArr[2];

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

		private HttpDeviceInquirer HttpInquiryStart(List<HttpDeviceInfo> httpList, ServerInfo sInfo)
		{
			HttpDeviceInquirer inquirer = new HttpDeviceInquirer(httpList, sInfo);
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

		void LoadIniFile()
		{
			string host = IniFile.IniReadValue("DB_SERVER_INFO", "HOST");
			string[] arr = host.Split('.');
			if (4 == arr.Length)
			{
				tbxIP1.Text = arr[0];
				tbxIP2.Text = arr[1];
				tbxIP3.Text = arr[2];
				tbxIP4.Text = arr[3];
			}
			tbxPortNum.Text = IniFile.IniReadValue("DB_SERVER_INFO", "PORT");
			tbxDBName.Text = IniFile.IniReadValue("DB_SERVER_INFO", "DB_NAME");
			tbxUsrName.Text = IniFile.IniReadValue("DB_SERVER_INFO", "USR_NAME");
			tbxPassword.Text = IniFile.IniReadValue("DB_SERVER_INFO", "PASSWORD");

			tbxServiceAreaNum.Text = IniFile.IniReadValue("SERVICE_AREA_INFO", "SERVICE_AREA_NUM");
			tbxUpdatePeriod.Text = IniFile.IniReadValue("SETTING", "UPDATE_PERIOD");

			string listView1ColNames = IniFile.IniReadValue("LISTVIEW_COLUMN", "LISTVIEW_1_COLUMN_NAME");
			string listView1ColWidths = IniFile.IniReadValue("LISTVIEW_COLUMN", "LISTVIEW_1_COLUMN_WIDTH");
			AddListViewColumns(listView1, listView1ColNames, listView1ColWidths);

			string listView2ColNames = IniFile.IniReadValue("LISTVIEW_COLUMN", "LISTVIEW_2_COLUMN_NAME");
			string listView2ColWidths = IniFile.IniReadValue("LISTVIEW_COLUMN", "LISTVIEW_2_COLUMN_WIDTH");
			AddListViewColumns(listView2, listView2ColNames, listView2ColWidths);
		}

		void AddListViewColumns(ListView ctrl, string nameStr, string widthStr)
		{
			string[] namesArr = nameStr.Split(',');
			string[] widthsArr = widthStr.Split(',');
			if (namesArr.Length == widthsArr.Length)
			{
				for (int i = 0; i < namesArr.Length; i++)
				{
					string colName = namesArr[i].Trim();
					string colWidth = widthsArr[i].Trim();
					int width;
					if (int.TryParse(colWidth, out width))
					{
						ctrl.Columns.Add(namesArr[i].Trim(), width);
					}
					else
					{
						ctrl.Columns.Add(namesArr[i].Trim());
					}
				}
			}

		}

		void SaveIniFile()
		{
			if (	IPValueCheck(tbxIP1.Text)
				&&	IPValueCheck(tbxIP2.Text)
				&&	IPValueCheck(tbxIP3.Text)
				&&	IPValueCheck(tbxIP4.Text))
			{
				string host = tbxIP1.Text + "." + tbxIP2.Text + "." + tbxIP3.Text + "." + tbxIP4.Text;
				IniFile.IniWriteValue("DB_SERVER_INFO", "HOST", host);
			}

			IniFile.IniWriteValue("DB_SERVER_INFO", "PORT", tbxPortNum.Text);
			IniFile.IniWriteValue("DB_SERVER_INFO", "DB_NAME", tbxDBName.Text);
			IniFile.IniWriteValue("DB_SERVER_INFO", "USR_NAME", tbxUsrName.Text);
			IniFile.IniWriteValue("DB_SERVER_INFO", "PASSWORD", tbxPassword.Text);

			IniFile.IniWriteValue("SERVICE_AREA_INFO", "SERVICE_AREA_NUM", tbxServiceAreaNum.Text);
			IniFile.IniWriteValue("SETTING", "UPDATE_PERIOD", tbxUpdatePeriod.Text);
		}

		private void listView1_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			btnEdit1_Click(sender, e);
		}

		private void listView2_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			btnEdit2_Click(sender, e);
		}

    }
}
