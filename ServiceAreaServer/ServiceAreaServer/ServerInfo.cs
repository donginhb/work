using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServiceAreaServer
{
	class ServerInfo
	{
		string _host_name;

		public string Host_name
		{
			get { return _host_name; }
			set { _host_name = value; }
		}
		int _port_num;

		public int Port_num
		{
			get { return _port_num; }
			set { _port_num = value; }
		}
		string _db_name;

		public string Db_name
		{
			get { return _db_name; }
			set { _db_name = value; }
		}
		string _user_id;

		public string User_id
		{
			get { return _user_id; }
			set { _user_id = value; }
		}
		string _pass_word;

		public string Pass_word
		{
			get { return _pass_word; }
			set { _pass_word = value; }
		}

		public ServerInfo(string hostName, int port, string dbName, string usr, string pwd)
		{
			Host_name = hostName;
			Port_num = port;
			Db_name = dbName;
			User_id = usr;
			Pass_word = pwd;
		}
	}
}
