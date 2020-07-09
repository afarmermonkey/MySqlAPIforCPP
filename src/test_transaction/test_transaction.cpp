/************************FILE INFOMATION***********************************
**
** Project Project       : C++Mysql8.0数据库跨平台编程实战
** Contact               : 913902738@qq.com
** Author                : 朱文泉
** Function              ：测试事务
***************************************************************************/


#include<iostream>
#include<mysql.h>
#include<thread>
#include<string>
#include<sstream>
#include<map>
#include<chrono>
using namespace std;
using namespace chrono;
int main()
{
	MYSQL mysql;
	mysql_init(&mysql);//初始化MySQL

	const char* host = "192.168.37.130";
	const char* user = "root";
	const char* passwd = "654321";
	const char* db = "zwq";//数据库名

	//支持多条sql语句
	if(!mysql_real_connect(&mysql, host, user, passwd, db, 3306, 0, CLIENT_MULTI_STATEMENTS))
	{
		std::cout << "mysql connect failed" << mysql_error(&mysql)<<std::endl;
	}
	else
	{
		std::cout << "mysql connect" << host << "  success!" << std::endl;
		
	}
	//1创建表
	string sql = "";	
	sql = "create table if not exists `t_video`(\
		`id` int auto_increment,\
		`name` varchar(1024),\
		`path` varchar(2046),\
		`size` int,\
		primary key(`id`)\
		)ENGINE=InnoDB;"; //设置支持回滚事务的引擎
	int re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed!" << mysql_error(&mysql);
	}

	//清理表中数据
	sql = "truncate t_video";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed!" << mysql_error(&mysql);
	}
	//事务
	//1开始事务
	//START TRANSACTION;
	sql = "START TRANSACTION";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed!" << mysql_error(&mysql);
	}
	//2设置为手动提交事务
	//set autocommit = 0
	sql = " set autocommit = 0 ";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed!" << mysql_error(&mysql);
	}

	//3 sql语句

	//插入三条语句，回滚
	for (int i = 0; i < 3; i++)
	{
		sql = " insert into t_video (`name`,`path`,`size`) values('test three!','path_test',5000) ";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "mysql_query failed!" << mysql_error(&mysql);
		}

	}
	

	//4 回滚ROLLBACK  MYISAM 不支持
	sql = "ROLLBACK";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed!" << mysql_error(&mysql);
	}
	//回滚后在提交数据
	for (int i = 0; i < 1000; i++)
	{
		sql = " insert into t_video (`name`,`path`,`size`) values('test three!','path_test',5000) ";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "mysql_query failed!" << mysql_error(&mysql);
		}

	}
	//5 COMMIT
	sql = " COMMIT ";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed!" << mysql_error(&mysql);
	}

	//6 恢复自动提交 set autocommit = 1
	sql = "set autocommit = 1";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed!" << mysql_error(&mysql);
	}


	//select
	sql = " select count(*) from t_video ";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed!" << mysql_error(&mysql);
	}
	MYSQL_RES* res = mysql_store_result(&mysql);
	if (res)
	{
		MYSQL_ROW row = mysql_fetch_row(res);
		if (row)
		{
			cout << "t_video count(*) = " << row[0] << endl;
		}
	}

	
	//单条语句插入10000条数据
	auto start = system_clock::now();
	for (int i = 0; i < 10000; i++)
	{
		sql = " insert into t_video (`name`,`path`,`size`) values('test three!',\
				'123456789012345678901234567890123456789012345678901234567890123\
				4567890123456789012345678901234567890',5000) ";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "mysql_query failed!" << mysql_error(&mysql);
		}

	}
	auto end = system_clock::now();
	auto dur = duration_cast<milliseconds>(end - start);
	cout << "1 单条语句插入10000条数据" << dur.count()/1000. << "秒"<<endl;
	//多条语句插入10000条数据
	{
		auto start = system_clock::now();
		sql = "";
		for (int i = 0; i < 10000; i++)
		{
			sql += " insert into t_video (`name`,`path`,`size`) values('test three!',\
				'123456789012345678901234567890123456789012345678901234567890123\
				4567890123456789012345678901234567890',5000); ";
		}
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "mysql_query failed!" << mysql_error(&mysql);
		}
		//多条语句时，只发送语句，不等待结果，需要获取结果集才能测试性能
		do {
			cout << mysql_affected_rows(&mysql) << flush;
		} while (mysql_next_result(&mysql) == 0);
		cout << endl;
		auto end = system_clock::now();
		auto dur = duration_cast<milliseconds>(end - start);
		cout << "2 多条语句插入10000条数据" << dur.count() / 1000. << "秒" << endl;
	}

	//事务插入10000条数据
	{
		auto start = system_clock::now();
		//1开始事务
		//START TRANSACTION;
		sql = "START TRANSACTION";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "mysql_query failed!" << mysql_error(&mysql);
		}
		//2设置为手动提交事务
		//set autocommit = 0
		sql = " set autocommit = 0 ";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "mysql_query failed!" << mysql_error(&mysql);
		}

		//3 sql语句
		//单条语句插入10000条
		for (int i = 0; i < 10000; i++)
		{
			sql = " insert into t_video (`name`,`path`,`size`) values('test three!','path_test',5000) ";
			re = mysql_query(&mysql, sql.c_str());
			if (re != 0)
			{
				cout << "mysql_query failed!" << mysql_error(&mysql);
			}
			else
			{
				//cout << mysql_affected_rows(&mysql) << flush;
			}
		}
		cout << endl;
		//5 COMMIT
		sql = " COMMIT ";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "mysql_query failed!" << mysql_error(&mysql);
		}

		//6 恢复自动提交 set autocommit = 1
		sql = "set autocommit = 1";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "mysql_query failed!" << mysql_error(&mysql);
		}
		auto end = system_clock::now();
		auto dur = duration_cast<milliseconds>(end - start);
		cout << "3 事务插入10000条数据" << dur.count() / 1000. << "秒" << endl;
	}

	mysql_close(&mysql);
	mysql_library_end();
	std::cout << "MySQL8.0 API!\n";
	//system("pause");
	return 0;
}