using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ServiceAreaClient
{
	public partial class DeviceEditForm : Form
	{
		public DeviceEditForm(int p_count)
		{
			InitializeComponent();

			ParaCount = p_count;
		}

		// 编辑参数的个数
		int _paraCount;

		public int ParaCount
		{
			get { return _paraCount; }
			set { _paraCount = value; }
		}

		// 编辑参数列表
		List<string> _paraList = new List<string>();

		public List<string> ParaList
		{
			get { return _paraList; }
			set { _paraList = value; }
		}

		private void btnOK_Click(object sender, EventArgs e)
		{
			ParaList.Clear();
			string[] arr = textBox1.Text.Split('\n');
			foreach (string str in arr)
			{
				string paraStr = str.Trim();
				if (string.Empty != paraStr)
				{
					ParaList.Add(str.Trim());
				}
			}
			if (ParaCount != ParaList.Count)
			{
				string msg = string.Format("参数个数不相等! 要求: {0}, 实际{1}", ParaCount, ParaList.Count);
				MessageBox.Show(msg);
				btnOK.DialogResult = System.Windows.Forms.DialogResult.Abort;
				ParaList.Clear();
				return;
			}
			else
			{
				btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			}
		}
	}
}
