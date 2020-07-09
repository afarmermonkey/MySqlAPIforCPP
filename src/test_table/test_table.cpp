#include<iostream>
#include<mysql.h>
#include<thread>
#include<string>
#include<sstream>
#include<map>
using namespace std;

int main()
{
	MYSQL mysql;
	mysql_init(&mysql);//初始化MySQL

	const char* host = "192.168.37.130";
	const char* user = "root";
	const char* passwd = "654321";
	const char* db = "zwq";//数据库名

	if(!mysql_real_connect(&mysql, host, user, passwd, db, 3306, 0, 0))
	{
		std::cout << "mysql connect failed" << mysql_error(&mysql)<<std::endl;
	}
	else
	{
		std::cout << "mysql connect" << host << "success!" << std::endl;
	}
	//1创建表
	string sql = "";	
	sql = "create table if not exists `t_image`(\
		`id` int auto_increment,\
		`name` varchar(1024),\
		`path` varchar(2046),\
		`size` int,\
		primary key(`id`)\
		)";
	int re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed!" << mysql_error(&mysql);
	}
	else
	{
		cout << "mysql_query success! " << endl;
	}
	//2插入数据 CLIENT_MULTI_STATEMENTS
	for (int i = 0; i < 10; i++)
	{
		//sql = "insert `t_image`(`name`,`path`,`size`) values('test.jpg','d:/img/test.jpg',10240)";
		stringstream ss;
		ss << "insert `t_image`(`name`,`path`,`size`) values('image";
		ss << i << ".jpg'" << ",'d:/img',10240)";
		sql = ss.str();
		re = mysql_query(&mysql, sql.c_str());
		if (re == 0)
		{
			int rownum = mysql_affected_rows(&mysql);
			cout << "mysql_affected_rows\t" << rownum << " id=" << mysql_insert_id(&mysql) << endl;
		}
		else
		{
			cout << "insert failed!" << mysql_error(&mysql) << endl;
		}
	}
	

	//3修改数据
	//UPDATE `zwq`.`t_image` SET `size` = '3000' WHERE (`id` = '3031');
	sql = "UPDATE `zwq`.`t_image` SET `name` = 'test2' WHERE (`id` = '2')";
	re = mysql_query(&mysql, sql.c_str());
	if (re == 0)
	{
		int rownum = mysql_affected_rows(&mysql);
		cout << "update mysql_affected_rows\t" << rownum << endl;
	}
	else
	{
		cout << "update failed!" << mysql_error(&mysql) << endl;
	}


	//根据map自动生成sql语句
	map<string, string> kv;
	kv.insert(make_pair("name", "image_update001.jpg"));
	kv.insert(make_pair("size", "5000"));
	string where = " where id=4 ";
	string temp = "";
	for (auto ptr = kv.begin(); ptr!=kv.end(); ptr++)
	{
		temp += "`";
		temp += ptr->first;
		temp += "`='";
		temp += ptr->second;
		temp += "',";
	}
	temp += "id=id";//这一句是避免temp语句最后是逗号，也可以删除最后一个逗号
	sql = " update t_image set ";
	sql += temp;
	sql += where;
	re = mysql_query(&mysql, sql.c_str());
	if (re == 0)
	{
		int rownum = mysql_affected_rows(&mysql);
		cout << "update mysql_affected_rows\t" << rownum << endl;
	}
	else
	{
		cout << "update failed!" << mysql_error(&mysql) << endl;
	}

	//4删除数据
	//delete不会删除空间，只做了标识（数据不见了，但内存还占用）
	sql = "delete from t_image where id=1";//可以选择删除条件delete from t_image where id=1
	re = mysql_query(&mysql, sql.c_str());
	if (re == 0)
	{
		int rownum = mysql_affected_rows(&mysql);
		cout << "delete mysql_affected_rows\t" << rownum << endl;
	}
	else
	{
		cout << "delete failed!" << mysql_error(&mysql) << endl;
	}
	//实际清理了空间
	sql = "OPTIMIZE TABLE t_image where id=1";
	re = mysql_query(&mysql, sql.c_str());

	//清空数据，并恢复id从1自增
	sql = "truncate t_image";//delete from t_image where id=1
	re = mysql_query(&mysql, sql.c_str());
	if (re == 0)
	{
		int rownum = mysql_affected_rows(&mysql);
		cout << "truncate mysql_affected_rows\t" << rownum << endl;
	}
	else
	{
		cout << "truncate failed!" << mysql_error(&mysql) << endl;
	}
	mysql_library_end();
	std::cout << "MySQL8.0 API!\n";
	//system("pause");
	return 0;
}