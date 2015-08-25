using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data;
using MySql.Data.MySqlClient;

namespace ServiceAreaServer
{
	class DBConnectMySQL
	{
		ServerInfo _serverInfomation;

		internal ServerInfo ServerInfomation
		{
			get { return _serverInfomation; }
			set { _serverInfomation = value; }
		}

		public DBConnectMySQL(ServerInfo sInfo)
		{
			ServerInfomation = sInfo;
		}

		public MySqlConnection GetMySqlConnection()
		{
			string M_str_sqlcon = "server=" + ServerInfomation.Host_name + ";user id=" + ServerInfomation.User_id
				+ ";password=" + ServerInfomation.Pass_word + ";database=" + ServerInfomation.Db_name;
			MySqlConnection myCon = new MySqlConnection(M_str_sqlcon);
			return myCon;
		}

		public void ExecuteMySqlCommand(string cmdStr)
		{
			MySqlConnection mysqlcon = this.GetMySqlConnection();
			mysqlcon.Open();
			MySqlCommand mysqlcom = new MySqlCommand(cmdStr, mysqlcon);
			mysqlcom.ExecuteNonQuery();
			mysqlcom.Dispose();
			mysqlcon.Close();
			mysqlcon.Dispose();
		}

		public MySqlDataReader GetMySqlReader(string sqlStr)
		{
			MySqlConnection mysqlcon = this.GetMySqlConnection();
			MySqlCommand mysqlcom = new MySqlCommand(sqlStr, mysqlcon);
			mysqlcon.Open();
			MySqlDataReader mysqlreader = mysqlcom.ExecuteReader(CommandBehavior.CloseConnection);
			return mysqlreader;
		}
	}
}
