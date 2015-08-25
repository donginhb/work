using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

using System.Xml.Linq;
using System.Collections.Generic;

namespace ServiceAreaServer
{
	public static class IniFile
	{
		[DllImport("kernel32")]
		private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);
		[DllImport("kernel32")]
		private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

		private static string _fullname = GetModulePath() + "\\config.ini";

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

		static string GetModulePath()
		{
			string retStr = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
			int idx = -1;
			if ( -1 != (idx = retStr.LastIndexOf(@"\")) )
			{
				retStr = retStr.Remove(idx);
			}
			return retStr;
		}
	}

}
