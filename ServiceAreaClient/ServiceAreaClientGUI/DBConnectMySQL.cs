using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data;
using MySql.Data.MySqlClient;

namespace UIManager
{
    class DBConnectMySQL
    {
        string _db_name;
        string _host_name;
        string _user_id;
        string _pass_word;
        int _port_num;

        public DBConnectMySQL(string dbName = "service_area", string hostName = "114.215.104.146",
                              string userId = "sarea", string passWord = "Huachang2015!", int port = 3306)
        {
            _db_name = dbName;
            _host_name = hostName;
            _user_id = userId;
            _pass_word = passWord;
            _port_num = port;
        }

        public MySqlConnection GetMySqlConnection()
        {
            string M_str_sqlcon = "server=" + _host_name + ";user id=" + _user_id + ";password=" + _pass_word + ";database=" + _db_name;
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

