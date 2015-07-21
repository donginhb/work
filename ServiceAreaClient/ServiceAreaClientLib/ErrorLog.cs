using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;

namespace ServiceAreaClientLib
{
	class ErrorLog
	{
		string _logPath = "";

		public string LogPath
		{
			get { return _logPath; }
			set { _logPath = value; }
		}

		public ErrorLog(string path)
		{
			LogPath = path;
		}

		public void LogAppend(string logText)
		{
			if (!Directory.Exists(LogPath))
			{
				return;
			}
			string logName = LogPath + "\\" + DateTime.Now.Year.ToString()
				+ DateTime.Now.Month.ToString().PadLeft(2, '0') + DateTime.Now.Day.ToString().PadLeft(2, '0') + ".log";
			StreamWriter sw = new StreamWriter(logName, true);
			sw.WriteLine(logText);
			sw.Close();
		}
	}
}
