using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

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

}
