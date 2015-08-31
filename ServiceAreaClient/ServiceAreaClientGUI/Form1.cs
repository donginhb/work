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
		/// <summary>
		/// 构造函数
		/// </summary>
        public Form1()
        {
            InitializeComponent();
            UIInit();
			updateDelegate = new UpdateEventHandler(OnUpdateEventProcess);
		}

		#region 全部字段

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

		private ServerInfo _db_server;		// 数据库服务器信息

		public ServerInfo Db_server
		{
			get { return _db_server; }
			set { _db_server = value; }
		}

		private ServerInfo _relay_server;	// 中继服务器信息

		public ServerInfo Relay_server
		{
			get { return _relay_server; }
			set { _relay_server = value; }
		}

		private int _service_area_num;		// 服务区编号

		public int Service_area_num
		{
			get { return _service_area_num; }
			set { _service_area_num = value; }
		}

		private int _update_period;			// 更新周期(分钟)

		public int Update_period
		{
			get { return _update_period; }
			set { _update_period = value; }
		}

		// 数据库的连接方式(直接连接数据库服务器, 或者经由中继服务器)
		private E_DB_CONNECT_MODE _db_connect_mode = E_DB_CONNECT_MODE.DIRECT;

		public E_DB_CONNECT_MODE Db_connect_mode
		{
			get { return _db_connect_mode; }
			set { _db_connect_mode = value; }
		}

		// 数据库各个表的名称列表
		private List<string> _db_table_list = new List<string>();

		public List<string> Db_table_list
		{
			get { return _db_table_list; }
			set { _db_table_list = value; }
		}

		#endregion

		#region UI按钮处理

		/// <summary>
		/// Start/Stop按钮
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void btnStart_Click(object sender, EventArgs e)
		{
			InquiryStart();
		}

		/// <summary>
		/// 设定窗体
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void btnSetting_Click(object sender, EventArgs e)
		{
			FormSetting fs = new FormSetting(Db_server, Relay_server, Service_area_num, Update_period, Db_connect_mode);
			if (DialogResult.OK == fs.ShowDialog())
			{
				Db_connect_mode = fs.Db_connect_mode;
				Db_server = fs.Db_server;
				Relay_server = fs.Relay_server;
				Service_area_num = fs.Service_area_num;
				Update_period = fs.Update_period;
			}
		}

		#endregion

		#region ListView控件右侧的"添加, 删除, 编辑"按钮事件

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
				SaveListViewContents();
			}
		}

		void ButtonDelClick(ListView list_view_ctrl)
		{
			bool changed = false;
			foreach (ListViewItem item in list_view_ctrl.SelectedItems)
			{
				list_view_ctrl.Items.Remove(item);
				changed = true;
			}
			if (changed)
			{
				SaveListViewContents();
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
				SaveListViewContents();
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
		private void listView4_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			btnEdit4_Click(sender, e);
		}

		private void listView5_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			btnEdit5_Click(sender, e);
		}

		#endregion

		#region 代理 & 事件
		UpdateEventHandler updateDelegate;
		#endregion

		//////////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 查询开始/终止
		/// </summary>
		void InquiryStart()
		{
			if ("Start" == btnStart.Text)
			{
                btnStart.Text = "Stop";
				UIEnable(false);

				// 1.生成查询设备列表
				List<ModbusDeviceInfo> electricMeterList = CreateElectricMeterList();
				// 2.查询开始
				ElectricMeterInquirer = ElectricMeterInquiryStart(electricMeterList);
				System.Threading.Thread.Sleep(100);

				List<PassengerCounterInfo> passengerCounterList = CreatePassengerCounterList();
				PassengerCounterInquirer = PassengerCounterInquiryStart(passengerCounterList);
				System.Threading.Thread.Sleep(100);

				List<ModbusDeviceInfo> roomThermometerList = CreateRoomThermometerList();
				RoomTemperatureInquirer = RoomTemperatureInquiryStart(roomThermometerList);
				System.Threading.Thread.Sleep(100);

				List<ModbusDeviceInfo> waterMeterList = CreateWaterMeterList();
				WaterMeterInquirer = WaterMeterInquiryStart(waterMeterList);
				System.Threading.Thread.Sleep(100);

				List<ModbusDeviceInfo> waterTemperatureList = CreateWaterTemperatureList();
				WaterTemperatureInquirer = WaterTemperatureInquiryStart(waterTemperatureList);
				System.Threading.Thread.Sleep(100);

				SaveIniFile();
			}
			else
			{
				// 停止查询
				ElectricMeterInquiryStop(ElectricMeterInquirer);
				PassengerCounterInquiryStop(PassengerCounterInquirer);
				RoomTemperatureInquiryStop(RoomTemperatureInquirer);
                WaterMeterInquiryStop(WaterMeterInquirer);
                WaterTemperatureInquiryStop(WaterTemperatureInquirer);

                btnStart.Text = "Start";
				UIEnable(true);
			}
		}

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

		/// <summary>
		/// UI控件有效/无效状态设定
		/// </summary>
		/// <param name="enable"></param>
        void UIEnable(bool enable)
        {
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

			btnSetting.Enabled = enable;

			// 单选框
            cbxAutoStart.Enabled = enable;
        }

		private void Form1_Load(object sender, EventArgs e)
		{
			if (cbxAutoStart.Checked)
			{
				System.Threading.Thread.Sleep(5000);
				InquiryStart();
			}
		}

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 如果处于查询过程中, 要停止查询
            ElectricMeterInquiryStop(ElectricMeterInquirer);
            PassengerCounterInquiryStop(PassengerCounterInquirer);
            RoomTemperatureInquiryStop(RoomTemperatureInquirer);
            WaterMeterInquiryStop(WaterMeterInquirer);
            WaterTemperatureInquiryStop(WaterTemperatureInquirer);

			SaveIniFile();
            SaveListViewContents();
        }

		private void SaveListViewContents()
		{
			List<ListView> ctrlList = new List<ListView>();
			ctrlList.Add(listView1);
			ctrlList.Add(listView2);
			ctrlList.Add(listView3);
			ctrlList.Add(listView4);
			ctrlList.Add(listView5);
			XmlFile.SaveListViewItems(ctrlList);
		}

		#region 创建各种类型设备的查询列表
		private List<ModbusDeviceInfo> CreateElectricMeterList()
		{
			List<ModbusDeviceInfo> electricMeterList = new List<ModbusDeviceInfo>();
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
				float fvalue;
				// 服务区编号
				deviceInfo.ServiceArea = Service_area_num;
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
				// 读数放大倍率
				if (float.TryParse(paraArr[5], out fvalue))
				{
					deviceInfo.Magnification = fvalue;
				}
				deviceInfo.DbTableName = Db_table_list[0];

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
				// 服务区编号
				deviceInfo.ServiceArea = Service_area_num;
				// 设备名称
				deviceInfo.DeviceName = paraArr[0];
				// DeviceSN
				deviceInfo.SpotNumber = paraArr[1];
				// Request String
				deviceInfo.RequestString1 = paraArr[2];
				deviceInfo.RequestString2 = paraArr[3];
				// 数据库中对应的表名
				deviceInfo.DbTableName = Db_table_list[1];

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
				deviceInfo.ServiceArea = Service_area_num;
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
				// 校正值
				float fAdjustment = 0;
				if (float.TryParse(paraArr[5], out fAdjustment))
				{
					deviceInfo.Adjustment = fAdjustment;
				}
				// 数据库中对应的表名
				deviceInfo.DbTableName = Db_table_list[2];

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
				float fvalue;
				// 服务区编号
				deviceInfo.ServiceArea = Service_area_num;
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
				if (float.TryParse(paraArr[5], out fvalue))
				{
					deviceInfo.Magnification = fvalue;
				}
				// 数据库中对应的表名
				deviceInfo.DbTableName = Db_table_list[3];

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
				deviceInfo.ServiceArea = Service_area_num;
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
				deviceInfo.DbTableName = Db_table_list[4];

				// 水温的量纲默认是10
				deviceInfo.Magnitude = 10;

				waterTemperatureMeterList.Add(deviceInfo);
			}
			return waterTemperatureMeterList;
		}
		
		#endregion

		#region 各种类型设备的查询开始/结束(InquiryStart/InquiryStop)动作

		/// <summary>
		/// 开始查询
		/// </summary>
		private ElectricMeterInquirer ElectricMeterInquiryStart(List<ModbusDeviceInfo> electricMeterList)
		{
			ElectricMeterInquirer inquirer = new ElectricMeterInquirer(electricMeterList, Db_server, Relay_server, Db_connect_mode);
			inquirer.CyclePeriod = Update_period;
			inquirer.TbxControl = textBox1;
			inquirer.StartInquiry(updateDelegate);

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

		private PassengerCounterInquirer PassengerCounterInquiryStart(List<PassengerCounterInfo> passengerCounterList)
		{
			PassengerCounterInquirer inquirer = new PassengerCounterInquirer(passengerCounterList, Db_server, Relay_server, Db_connect_mode);
			inquirer.CyclePeriod = Update_period;
			inquirer.TbxControl = textBox2;
			inquirer.StartInquiry(updateDelegate);

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

		private RoomTemperatureInquirer RoomTemperatureInquiryStart(List<ModbusDeviceInfo> roomThermometerList)
		{
			RoomTemperatureInquirer inquirer = new RoomTemperatureInquirer(roomThermometerList, Db_server, Relay_server, Db_connect_mode);
			inquirer.CyclePeriod = Update_period;
			inquirer.TbxControl = textBox3;
			inquirer.StartInquiry(updateDelegate);
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

		private WaterMeterInquirer WaterMeterInquiryStart(List<ModbusDeviceInfo> waterMeterList)
		{
			WaterMeterInquirer inquirer = new WaterMeterInquirer(waterMeterList, Db_server, Relay_server, Db_connect_mode);
			inquirer.CyclePeriod = Update_period;
			inquirer.TbxControl = textBox4;
			inquirer.StartInquiry(updateDelegate);
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

		private WaterTemperatureInquirer WaterTemperatureInquiryStart(List<ModbusDeviceInfo> waterTemperatureList)
		{
			WaterTemperatureInquirer inquirer = new WaterTemperatureInquirer(waterTemperatureList, Db_server, Relay_server, Db_connect_mode);
			inquirer.CyclePeriod = Update_period;
			inquirer.TbxControl = textBox5;
			inquirer.StartInquiry(updateDelegate);
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
		
		#endregion

		void LoadIniFile()
		{
			Db_server = new ServerInfo("", 0, "", "", "");
			Db_server.Host_name = IniFile.IniReadValue("DB_SERVER_INFO", "HOST");
			string readStr = IniFile.IniReadValue("DB_SERVER_INFO", "PORT");
			int value;
			if (int.TryParse(readStr, out value))
			{
				Db_server.Port_num = value;
			}
			Db_server.Db_name = IniFile.IniReadValue("DB_SERVER_INFO", "DB_NAME");
			Db_server.User_id = IniFile.IniReadValue("DB_SERVER_INFO", "USR_NAME");
			Db_server.Pass_word = IniFile.IniReadValue("DB_SERVER_INFO", "PASSWORD");

			Relay_server = new ServerInfo("", 0, "", "", "");
			Relay_server.Host_name = IniFile.IniReadValue("RELAY_SERVER_INFO", "HOST");
			readStr = IniFile.IniReadValue("RELAY_SERVER_INFO", "PORT");
			if (int.TryParse(readStr, out value))
			{
				Relay_server.Port_num = value;
			}

			readStr = IniFile.IniReadValue("SERVICE_AREA_INFO", "SERVICE_AREA_NUM");
			if (int.TryParse(readStr, out value))
			{
				Service_area_num = value;
			}
			readStr = IniFile.IniReadValue("SETTING", "UPDATE_PERIOD");
			if (int.TryParse(readStr, out value))
			{
				Update_period = value;
			}

            readStr = IniFile.IniReadValue("DB_CONNECT_INFO", "CONNECT_MODE");
            if (readStr == E_DB_CONNECT_MODE.DIRECT.ToString())
            {
                Db_connect_mode = E_DB_CONNECT_MODE.DIRECT;
            }
            else
            {
                Db_connect_mode = E_DB_CONNECT_MODE.RELAY;
            }

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

			Db_table_list = new List<string>();
			string table_name = IniFile.IniReadValue("DB_TABLE_NAME", "DB_TABLE_1"); Db_table_list.Add(table_name);
			table_name = IniFile.IniReadValue("DB_TABLE_NAME", "DB_TABLE_2"); Db_table_list.Add(table_name);
			table_name = IniFile.IniReadValue("DB_TABLE_NAME", "DB_TABLE_3"); Db_table_list.Add(table_name);
			table_name = IniFile.IniReadValue("DB_TABLE_NAME", "DB_TABLE_4"); Db_table_list.Add(table_name);
			table_name = IniFile.IniReadValue("DB_TABLE_NAME", "DB_TABLE_5"); Db_table_list.Add(table_name);

			string autoStartStr = IniFile.IniReadValue("SETTING", "AUTO_START");
			if ("true" == autoStartStr.ToLower())
			{
				cbxAutoStart.Checked = true;
			}
			else
			{
				cbxAutoStart.Checked = false;
			}
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
				IniFile.IniWriteValue("DB_SERVER_INFO", "HOST", Db_server.Host_name);

				IniFile.IniWriteValue("DB_SERVER_INFO", "PORT", Db_server.Port_num.ToString());
				IniFile.IniWriteValue("DB_SERVER_INFO", "DB_NAME", Db_server.Db_name);
				IniFile.IniWriteValue("DB_SERVER_INFO", "USR_NAME", Db_server.User_id);
				IniFile.IniWriteValue("DB_SERVER_INFO", "PASSWORD", Db_server.Pass_word);

				IniFile.IniWriteValue("SERVICE_AREA_INFO", "SERVICE_AREA_NUM", Service_area_num.ToString());
				IniFile.IniWriteValue("SETTING", "UPDATE_PERIOD", Update_period.ToString());
				IniFile.IniWriteValue("SETTING", "AUTO_START", cbxAutoStart.Checked.ToString());

				IniFile.IniWriteValue("RELAY_SERVER_INFO", "HOST", Relay_server.Host_name);
				IniFile.IniWriteValue("RELAY_SERVER_INFO", "PORT", Relay_server.Port_num.ToString());

				IniFile.IniWriteValue("DB_CONNECT_INFO", "CONNECT_MODE", Db_connect_mode.ToString());
			}
			catch (Exception ex)
			{
				System.Diagnostics.Trace.WriteLine(ex.ToString());
			}
		}

		/// <summary>
		/// 程序更新事件处理
		/// </summary>
		void OnUpdateEventProcess()
		{
			// 启动更新程序
			// 自身关闭退出
			MessageBox.Show("我感受到Update事件发生了!");
			this.Close();
		}
	}
}
