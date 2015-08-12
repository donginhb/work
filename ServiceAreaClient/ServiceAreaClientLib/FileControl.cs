using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

using System.Xml.Linq;
using System.Windows.Forms;
using System.Collections.Generic;

namespace ServiceAreaClientLib
{
	public static class IniFile
	{
		[DllImport("kernel32")]
		private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);
		[DllImport("kernel32")]
		private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

		private static string _fullname = System.Windows.Forms.Application.StartupPath + "\\config.ini";

		public static string Fullname
		{
			get { return IniFile._fullname; }
			set { IniFile._fullname = value; }
		}

		public static void IniWriteValue(string Section, string Key, string Value)
		{
			WritePrivateProfileString(Section, Key, Value, Fullname);
		}

		public static string IniReadValue(string Section, string Key)
		{
			StringBuilder temp = new StringBuilder(500);
			int i = GetPrivateProfileString(Section, Key, "", temp, 500, Fullname);
			return temp.ToString();
		}
	}

	public static class XmlFile
	{
		private static string _fullname = System.Windows.Forms.Application.StartupPath + "\\data.xml";

		public static string Fullname
		{
			get { return XmlFile._fullname; }
			set { XmlFile._fullname = value; }
		}
		public static void SaveListViewItems(List<ListView> listViewCtrlList)
		{
			try
			{
				XDocument xdoc = new XDocument();
				XElement root = new XElement("ListViewContents");
				foreach (ListView ctrl in listViewCtrlList)
				{
					List<string> colNameList = new List<string>();
					foreach (ColumnHeader col in ctrl.Columns)
					{
						colNameList.Add(col.Text);
					}
					XElement xElmt = new XElement(ctrl.Name);
					int rowIdx = 0;
					foreach (ListViewItem item in ctrl.Items)
					{
						XElement xRowElmt = new XElement("Row_" + rowIdx.ToString());
						XAttribute xAtbChecked = new XAttribute("Checked", item.Checked);
						xRowElmt.Add(xAtbChecked);
						int colIdx = 0;
						foreach (ListViewItem.ListViewSubItem subItem in item.SubItems)
						{
							XElement xColElmt = new XElement(colNameList[colIdx], subItem.Text);
							xRowElmt.Add(xColElmt);
							colIdx++;
						}
						xElmt.Add(xRowElmt);
						rowIdx++;
					}
					root.Add(xElmt);
				}
				xdoc.Add(root);
				xdoc.Save(Fullname);
			}
			catch (Exception ex)
			{
				System.Diagnostics.Trace.WriteLine(ex.ToString());
			}
		}

		public static void LoadListViewItems(List<ListView> listViewCtrlList)
		{
            try
            {
                XDocument xdoc = XDocument.Load(Fullname);
                foreach (ListView ctrl in listViewCtrlList)
                {
                    foreach (XElement xElmt in xdoc.Elements("ListViewContents").Elements())
                    {
                        if (ctrl.Name == xElmt.Name)
                        {
                            ctrl.Items.Clear();
                            foreach (XElement rowItem in xElmt.Elements())
                            {
                                ListViewItem item = null;
                                foreach (XElement colItem in rowItem.Elements())
                                {
                                    if (null == item)
                                    {
                                        item = new ListViewItem(colItem.Value);
                                        if ("true" == rowItem.Attribute("Checked").Value.ToLower())
                                        {
                                            item.Checked = true;
                                        }
                                    }
                                    else
                                    {
                                        item.SubItems.Add(colItem.Value);
                                    }
                                }
                                ctrl.Items.Add(item);
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Trace.WriteLine(ex.ToString());
            }
		}
	}

}
