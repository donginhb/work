using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;

namespace ServiceAreaClientLib
{
	public class LogFileOutput
	{
		static string _logName = "log.txt";

		public static string LogName
		{
			get { return _logName; }
			set { _logName = value; }
		}

		static bool _enabled = false;

		public static bool Enabled
		{
			get { return LogFileOutput._enabled; }
			set { LogFileOutput._enabled = value; }
		}


		public static void LogAppend(string logText)
		{
			if (false == Enabled)
			{
				return;
			}
			string dateTimeStr = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
			StreamWriter sw = new StreamWriter(LogName, true);
			sw.WriteLine(dateTimeStr + ":\t\t" + logText);
			sw.Close();
		}
	}
}
