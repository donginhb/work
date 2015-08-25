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

namespace ServiceAreaClient
{
	public partial class FormSetting : Form
	{
		public FormSetting( ServerInfo db_server, ServerInfo relay_server, int sarea_num,
                            int update_period, E_DB_CONNECT_MODE connect_mode)
		{
			InitializeComponent();
			string host = db_server.Host_name;
			string[] arr = host.Split('.');
			if (4 == arr.Length)
			{
				tbxIP1.Text = arr[0];
				tbxIP2.Text = arr[1];
				tbxIP3.Text = arr[2];
				tbxIP4.Text = arr[3];
			}
			tbxPortNum1.Text = db_server.Port_num.ToString();
			tbxDBName.Text = db_server.Db_name;
			tbxUsrName.Text = db_server.User_id;
			tbxPassword.Text = db_server.Pass_word;

			host = relay_server.Host_name;
			arr = host.Split('.');
			if (4 == arr.Length)
			{
				tbxIP5.Text = arr[0];
				tbxIP6.Text = arr[1];
				tbxIP7.Text = arr[2];
				tbxIP8.Text = arr[3];
			}
			tbxPortNum2.Text = relay_server.Port_num.ToString();

			tbxServiceAreaNum.Text = sarea_num.ToString();
			Service_area_num = sarea_num;
			tbxUpdatePeriod.Text = update_period.ToString();
			Update_period = update_period;

            Db_server = db_server;
            Relay_server = relay_server;

            Db_connect_mode = connect_mode;
            if (E_DB_CONNECT_MODE.DIRECT == Db_connect_mode)
            {
                checkBox1.Checked = true;
            }
            else
            {
                checkBox2.Checked = true;
            }
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

		private E_DB_CONNECT_MODE _db_connect_mode = E_DB_CONNECT_MODE.DIRECT;

		public E_DB_CONNECT_MODE Db_connect_mode
		{
			get { return _db_connect_mode; }
			set { _db_connect_mode = value; }
		}

		int _service_area_num;				// 服务区编号

		public int Service_area_num
		{
			get { return _service_area_num; }
			set { _service_area_num = value; }
		}

		int _update_period;					// 更新周期(分)

		public int Update_period
		{
			get { return _update_period; }
			set { _update_period = value; }
		}

		private void checkBox1_CheckedChanged(object sender, EventArgs e)
		{
			if (checkBox1.Checked)
			{
				groupBox1.Enabled = true;
				groupBox2.Enabled = false;
				checkBox2.Checked = false;

				Db_connect_mode = E_DB_CONNECT_MODE.DIRECT;
			}
		}

		private void checkBox2_CheckedChanged(object sender, EventArgs e)
		{
			if (checkBox2.Checked)
			{
				groupBox2.Enabled = true;
				groupBox1.Enabled = false;
				checkBox1.Checked = false;

				Db_connect_mode = E_DB_CONNECT_MODE.RELAY;
			}
		}

		private void button1_Click(object sender, EventArgs e)
		{
            Db_server.Host_name = tbxIP1.Text + "." + tbxIP2.Text + "." + tbxIP3.Text + "." + tbxIP4.Text;
            int value;
            if (int.TryParse(tbxPortNum1.Text, out value))
	        {
		        Db_server.Port_num = value;
	        }
            Db_server.Db_name = tbxDBName.Text;
            Db_server.User_id = tbxUsrName.Text;
            Db_server.Pass_word = tbxPassword.Text;

            Relay_server.Host_name = tbxIP5.Text + "." + tbxIP6.Text + "." + tbxIP7.Text + "." + tbxIP8.Text;
            if (int.TryParse(tbxPortNum2.Text, out value))
            {
                Relay_server.Port_num = value;
            }

            if (int.TryParse(tbxServiceAreaNum.Text, out value))
	        {
                Service_area_num = value;
	        }
            if (int.TryParse(tbxUpdatePeriod.Text, out value))
            {
                Update_period = value;
            }
		}

		/// <summary>
		/// UI参数检查
		/// </summary>
		/// <returns></returns>
		bool CheckUIValue()
		{
			if (
				   !IPValueCheck(tbxIP1.Text)
				|| !IPValueCheck(tbxIP2.Text)
				|| !IPValueCheck(tbxIP3.Text)
				|| !IPValueCheck(tbxIP4.Text))
			{
				return false;
			}
			int val;
			if (!int.TryParse(tbxPortNum1.Text, out val))
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
			if ((string.Empty == ipStr)
				|| !int.TryParse(ipStr, out ipVal))
			{
				return false;
			}
			if (ipVal < 0
				|| ipVal > 255)
			{
				return false;
			}
			return true;
		}

	}

}
