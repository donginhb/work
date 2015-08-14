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
using ServiceAreaClientLib;
using ServiceAreaClientLib.DeviceInquirer;

namespace ServiceAreaClient
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            UIInit();
        }

        ElectricMeterInquirer _electricMeterInquirer = null;

        public ElectricMeterInquirer ElectricMeterInquirer
        {
            get { return _electricMeterInquirer; }
            set { _electricMeterInquirer = value; }
        }

		PassengerCounterInquirer _passengerCounterInquirer = null;

		public PassengerCounterInquirer PassengerCounterInquirer
        {
            get { return _passengerCounterInquirer; }
            set { _passengerCounterInquirer = value; }
        }

		RoomTemperatureInquirer _roomTemperatureInquirer = null;

		public RoomTemperatureInquirer RoomTemperatureInquirer
		{
			get { return _roomTemperatureInquirer; }
			set { _roomTemperatureInquirer = value; }
		}

		WaterMeterInquirer _waterMeterInquirer = null;

		public WaterMeterInquirer WaterMeterInquirer
		{
			get { return _waterMeterInquirer; }
			set { _waterMeterInquirer = value; }
		}
		WaterTemperatureInquirer _waterTemperatureInquirer = null;

		public WaterTemperatureInquirer WaterTemperatureInquirer
		{
			get { return _waterTemperatureInquirer; }
			set { _waterTemperatureInquirer = value; }
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
				List<ModbusDeviceInfo> electricMeterList = CreateElectricMeterList();
				// 2.查询开始
				ElectricMeterInquirer = ElectricMeterInquiryStart(electricMeterList, sInfo);
				System.Threading.Thread.Sleep(100);

				List<PassengerCounterInfo> passengerCounterList = CreatePassengerCounterList();
				PassengerCounterInquirer = PassengerCounterInquiryStart(passengerCounterList, sInfo);
				System.Threading.Thread.Sleep(100);

				List<ModbusDeviceInfo> roomThermometerList = CreateRoomThermometerList();
				RoomTemperatureInquirer = RoomTemperatureInquiryStart(roomThermometerList, sInfo);
				System.Threading.Thread.Sleep(100);

				List<ModbusDeviceInfo> waterMeterList = CreateWaterMeterList();
				WaterMeterInquirer = WaterMeterInquiryStart(waterMeterList, sInfo);
				System.Threading.Thread.Sleep(100);

				List<ModbusDeviceInfo> waterTemperatureList = CreateWaterTemperatureList();
				WaterTemperatureInquirer = WaterTemperatureInquiryStart(waterTemperatureList, sInfo);
				System.Threading.Thread.Sleep(100);

				SaveIniFile();
            }
            else
            {
				// 停止查询
				ElectricMeterInquiryStop(ElectricMeterInquirer);
				PassengerCounterInquiryStop(PassengerCounterInquirer);
				RoomTemperatureInquiryStop(RoomTemperatureInquirer);
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
			ctrlList.Add(listView3);
			ctrlList.Add(listView4);
			ctrlList.Add(listView5);
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
			listView3.Enabled = enable;
			listView4.Enabled = enable;
			listView5.Enabled = enable;

            // 按钮
            btnAdd1.Enabled = enable;
            btnDel1.Enabled = enable;
            btnEdit1.Enabled = enable;
            btnAdd2.Enabled = enable;
            btnDel2.Enabled = enable;
            btnEdit2.Enabled = enable;
			btnAdd3.Enabled = enable;
			btnDel3.Enabled = enable;
			btnEdit3.Enabled = enable;
			btnAdd4.Enabled = enable;
			btnDel4.Enabled = enable;
			btnEdit4.Enabled = enable;
			btnAdd5.Enabled = enable;
			btnDel5.Enabled = enable;
			btnEdit5.Enabled = enable;
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
			ctrlList.Add(listView3);
			ctrlList.Add(listView4);
			ctrlList.Add(listView5);
			XmlFile.SaveListViewItems(ctrlList);
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

		private void btnAdd3_Click(object sender, EventArgs e)
		{
			ButtonAddClick(listView3);
		}

		private void btnDel3_Click(object sender, EventArgs e)
		{
			ButtonDelClick(listView3);
		}

		private void btnEdit3_Click(object sender, EventArgs e)
		{
			ButtonEditClick(listView3);
		}

		private void btnAdd4_Click(object sender, EventArgs e)
		{
			ButtonAddClick(listView4);
		}

		private void btnDel4_Click(object sender, EventArgs e)
		{
			ButtonDelClick(listView4);
		}

		private void btnEdit4_Click(object sender, EventArgs e)
		{
			ButtonEditClick(listView4);
		}

		private void btnAdd5_Click(object sender, EventArgs e)
		{
			ButtonAddClick(listView5);
		}

		private void btnDel5_Click(object sender, EventArgs e)
		{
			ButtonDelClick(listView5);
		}

		private void btnEdit5_Click(object sender, EventArgs e)
		{
			ButtonEditClick(listView5);
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

		private List<ModbusDeviceInfo> CreateElectricMeterList()
		{
			List<ModbusDeviceInfo>  electricMeterList = new List<ModbusDeviceInfo>();
			// 遍历ListView控件取得各个查询设备的参数情报
			// 首先是电表
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
				int value;
				// 服务区编号
				if (int.TryParse(tbxServiceAreaNum.Text, out value))
				{
					deviceInfo.ServiceArea = value;
				}
				// 设备名称
				deviceInfo.DeviceName = paraArr[0];
				// DeviceSN
				deviceInfo.SpotNumber = paraArr[1];
				// 设备编号
				if (int.TryParse(paraArr[2], out value))
				{
					deviceInfo.DeviceAddr = value;
				}
				// Host IP
				deviceInfo.HostName = paraArr[3];
				// 端口号
				if (int.TryParse(paraArr[4], out value))
				{
					deviceInfo.PortNum = value;
				}
				// 读数据起始地址
				if (int.TryParse(paraArr[5], out value))
				{
					deviceInfo.ReadAddr = value;
				}
				// 读数据长度
				if (int.TryParse(paraArr[6], out value))
				{
					deviceInfo.ReadLength = value;
				}
				// 读数放大倍率
				if (int.TryParse(paraArr[7], out value))
				{
					deviceInfo.Magnification = value;
				}
				deviceInfo.DbTableName = _db_table_list[0];

				// 电表的量纲默认是100
				deviceInfo.Magnitude = 100;

				// 加入到查询设备列表中
				electricMeterList.Add(deviceInfo);
			}

			return electricMeterList;
		}

		private List<PassengerCounterInfo> CreatePassengerCounterList()
		{
			List<PassengerCounterInfo> passengerCounterList = new List<PassengerCounterInfo>();
			// 然后是客流计数设备(摄像头)
			foreach (ListViewItem item in listView2.Items)
			{
				if (!item.Checked)
				{
					continue;
				}
				PassengerCounterInfo deviceInfo = new PassengerCounterInfo();
				string[] paraArr = new string[item.SubItems.Count];
				int idx = 0;
				foreach (ListViewItem.ListViewSubItem subitems in item.SubItems)
				{
					paraArr[idx] = subitems.Text.Trim();
					idx++;
				}
				int value;
				// 服务区编号
				if (int.TryParse(tbxServiceAreaNum.Text, out value))
				{
					deviceInfo.ServiceArea = value;
				}
				// 设备名称
				deviceInfo.DeviceName = paraArr[0];
				// DeviceSN
				deviceInfo.DeviceSn = paraArr[1];
				// Request String
				deviceInfo.RequestString1 = paraArr[2];
				deviceInfo.RequestString2 = paraArr[3];
				// 数据库中对应的表名
				deviceInfo.DbTableName = _db_table_list[1];

				passengerCounterList.Add(deviceInfo);
			}
			return passengerCounterList;
		}

		private List<ModbusDeviceInfo> CreateRoomThermometerList()
		{
			List<ModbusDeviceInfo> roomThermometerList = new List<ModbusDeviceInfo>();
			foreach (ListViewItem item in listView3.Items)
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
				int value;
				// 服务区编号
				if (int.TryParse(tbxServiceAreaNum.Text, out value))
				{
					deviceInfo.ServiceArea = value;
				}
				// 设备名称
				deviceInfo.DeviceName = paraArr[0];
				// DeviceSN
				deviceInfo.SpotNumber = paraArr[1];
				// 目标设备地址
				if (int.TryParse(paraArr[2], out value))
				{
					deviceInfo.DeviceAddr = value;
				}
				// IP
				deviceInfo.HostName = paraArr[3];
				// 端口号
				if (int.TryParse(paraArr[4], out value))
				{
					deviceInfo.PortNum = value;
				}
				// 数据库中对应的表名
				deviceInfo.DbTableName = _db_table_list[2];

				// 室温的量纲默认是100
				deviceInfo.Magnitude = 100;

				roomThermometerList.Add(deviceInfo);
			}
			return roomThermometerList;
		}

		private List<ModbusDeviceInfo> CreateWaterMeterList()
		{
			List<ModbusDeviceInfo> waterMeterList = new List<ModbusDeviceInfo>();
			foreach (ListViewItem item in listView4.Items)
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
				int value;
				// 服务区编号
				if (int.TryParse(tbxServiceAreaNum.Text, out value))
				{
					deviceInfo.ServiceArea = value;
				}
				// 设备名称
				deviceInfo.DeviceName = paraArr[0];
				// DeviceSN
				deviceInfo.SpotNumber = paraArr[1];
				// 目标设备地址
				if (int.TryParse(paraArr[2], out value))
				{
					deviceInfo.DeviceAddr = value;
				}
				// IP
				deviceInfo.HostName = paraArr[3];
				// 端口号
				if (int.TryParse(paraArr[4], out value))
				{
					deviceInfo.PortNum = value;
				}
				// 读数放大倍率
				if (int.TryParse(paraArr[5], out value))
				{
					deviceInfo.Magnification = value;
				}
				// 数据库中对应的表名
				deviceInfo.DbTableName = _db_table_list[3];

				// 水表的量纲默认是10
				deviceInfo.Magnitude = 10;

				waterMeterList.Add(deviceInfo);
			}
			return waterMeterList;
		}

		private List<ModbusDeviceInfo> CreateWaterTemperatureList()
		{
			List<ModbusDeviceInfo> waterTemperatureMeterList = new List<ModbusDeviceInfo>();
			foreach (ListViewItem item in listView5.Items)
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
				int value;
				// 服务区编号
				if (int.TryParse(tbxServiceAreaNum.Text, out value))
				{
					deviceInfo.ServiceArea = value;
				}
				// 设备名称
				deviceInfo.DeviceName = paraArr[0];
				// DeviceSN
				deviceInfo.SpotNumber = paraArr[1];
				// 目标设备地址
				if (int.TryParse(paraArr[2], out value))
				{
					deviceInfo.DeviceAddr = value;
				}
				// IP
				deviceInfo.HostName = paraArr[3];
				// 端口号
				if (int.TryParse(paraArr[4], out value))
				{
					deviceInfo.PortNum = value;
				}
				// 数据库中对应的表名
				deviceInfo.DbTableName = _db_table_list[4];

				// 水温的量纲默认是10
				deviceInfo.Magnitude = 10;

				waterTemperatureMeterList.Add(deviceInfo);
			}
			return waterTemperatureMeterList;
		}

		/// <summary>
		/// 开始查询
		/// </summary>
		private ElectricMeterInquirer ElectricMeterInquiryStart(List<ModbusDeviceInfo> electricMeterList, ServerInfo sInfo)
		{
            ElectricMeterInquirer inquirer = new ElectricMeterInquirer(electricMeterList, sInfo);
			int value;
			if (int.TryParse(tbxUpdatePeriod.Text, out value))
			{
				inquirer.CyclePeriod = value;
			}
			inquirer.TbxControl = textBox1;
			inquirer.StartInquiry();

			return inquirer;
		}

        void ElectricMeterInquiryStop(ElectricMeterInquirer inqurier)
		{
			if (null != inqurier)
			{
				inqurier.StopInquiry();
				inqurier = null;
			}
		}

		private PassengerCounterInquirer PassengerCounterInquiryStart(List<PassengerCounterInfo> passengerCounterList, ServerInfo sInfo)
		{
			PassengerCounterInquirer inquirer = new PassengerCounterInquirer(passengerCounterList, sInfo);
			int value;
			if (int.TryParse(tbxUpdatePeriod.Text, out value))
			{
				inquirer.CyclePeriod = value;
			}
			inquirer.TbxControl = textBox2;
			inquirer.StartInquiry();

			return inquirer;
		}

		void PassengerCounterInquiryStop(PassengerCounterInquirer inqurier)
		{
			if (null != inqurier)
			{
				inqurier.StopInquiry();
				inqurier = null;
			}
		}

		private RoomTemperatureInquirer RoomTemperatureInquiryStart(List<ModbusDeviceInfo> roomThermometerList, ServerInfo sInfo)
		{
			RoomTemperatureInquirer inquirer = new RoomTemperatureInquirer(roomThermometerList, sInfo);
			int value;
			if (int.TryParse(tbxUpdatePeriod.Text, out value))
			{
				inquirer.CyclePeriod = value;
			}
			inquirer.TbxControl = textBox3;
			inquirer.StartInquiry();
			return inquirer;
		}

		void RoomTemperatureInquiryStop(RoomTemperatureInquirer inqurier)
		{
			if (null != inqurier)
			{
				inqurier.StopInquiry();
				inqurier = null;
			}
		}

		private WaterMeterInquirer WaterMeterInquiryStart(List<ModbusDeviceInfo> waterMeterList, ServerInfo sInfo)
		{
			WaterMeterInquirer inquirer = new WaterMeterInquirer(waterMeterList, sInfo);
			int value;
			if (int.TryParse(tbxUpdatePeriod.Text, out value))
			{
				inquirer.CyclePeriod = value;
			}
			inquirer.TbxControl = textBox4;
			inquirer.StartInquiry();
			return inquirer;
		}

		void WaterMeterInquiryStop(WaterMeterInquirer inqurier)
		{
			if (null != inqurier)
			{
				inqurier.StopInquiry();
				inqurier = null;
			}
		}

		private WaterTemperatureInquirer WaterTemperatureInquiryStart(List<ModbusDeviceInfo> waterTemperatureList, ServerInfo sInfo)
		{
			WaterTemperatureInquirer inquirer = new WaterTemperatureInquirer(waterTemperatureList, sInfo);
			int value;
			if (int.TryParse(tbxUpdatePeriod.Text, out value))
			{
				inquirer.CyclePeriod = value;
			}
			inquirer.TbxControl = textBox5;
			inquirer.StartInquiry();
			return inquirer;
		}

		void WaterTemperatureInquiryStop(WaterTemperatureInquirer inqurier)
		{
			if (null != inqurier)
			{
				inqurier.StopInquiry();
				inqurier = null;
			}
		}

		// 数据库各个表的名称列表
		private List<string> _db_table_list = new List<string>();

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

			string listView3ColNames = IniFile.IniReadValue("LISTVIEW_COLUMN", "LISTVIEW_3_COLUMN_NAME");
			string listView3ColWidths = IniFile.IniReadValue("LISTVIEW_COLUMN", "LISTVIEW_3_COLUMN_WIDTH");
			AddListViewColumns(listView3, listView3ColNames, listView3ColWidths);

			string listView4ColNames = IniFile.IniReadValue("LISTVIEW_COLUMN", "LISTVIEW_4_COLUMN_NAME");
			string listView4ColWidths = IniFile.IniReadValue("LISTVIEW_COLUMN", "LISTVIEW_4_COLUMN_WIDTH");
			AddListViewColumns(listView4, listView4ColNames, listView4ColWidths);

			string listView5ColNames = IniFile.IniReadValue("LISTVIEW_COLUMN", "LISTVIEW_5_COLUMN_NAME");
			string listView5ColWidths = IniFile.IniReadValue("LISTVIEW_COLUMN", "LISTVIEW_5_COLUMN_WIDTH");
			AddListViewColumns(listView5, listView5ColNames, listView5ColWidths);

			_db_table_list = new List<string>();
			string table_name = IniFile.IniReadValue("DB_TABLE_NAME", "DB_TABLE_1"); _db_table_list.Add(table_name);
			table_name = IniFile.IniReadValue("DB_TABLE_NAME", "DB_TABLE_2"); _db_table_list.Add(table_name);
			table_name = IniFile.IniReadValue("DB_TABLE_NAME", "DB_TABLE_3"); _db_table_list.Add(table_name);
			table_name = IniFile.IniReadValue("DB_TABLE_NAME", "DB_TABLE_4"); _db_table_list.Add(table_name);
			table_name = IniFile.IniReadValue("DB_TABLE_NAME", "DB_TABLE_5"); _db_table_list.Add(table_name);
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
			try
			{
				if (IPValueCheck(tbxIP1.Text)
					&& IPValueCheck(tbxIP2.Text)
					&& IPValueCheck(tbxIP3.Text)
					&& IPValueCheck(tbxIP4.Text))
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
			catch (Exception ex)
			{
				System.Diagnostics.Trace.WriteLine(ex.ToString());
			}
		}

		private void listView1_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			btnEdit1_Click(sender, e);
		}

		private void listView2_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			btnEdit2_Click(sender, e);
		}

		private void listView3_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			btnEdit3_Click(sender, e);
		}

		private void btnTest_Click(object sender, EventArgs e)
		{
		}

	}
}
