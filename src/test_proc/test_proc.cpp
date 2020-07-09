/************************FILE INFOMATION***********************************
**
** Project Project       : C++Mysql8.0数据库跨平台编程实战
** Contact               : 913902738@qq.com
** Author                : 朱文泉
** Function              ：测试存储过程
***************************************************************************/

#include<iostream>
#include<mysql.h>
#include<thread>
#include<string>
#include<sstream>
#include<map>
#include<chrono>
#include<fstream>
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
	string sql = "";
	//1 创建存储过程
	sql = " CREATE PROCEDURE `p_1`(\
			IN p_in INT,\
			OUT p_out INT,\
			INOUT p_inout INT)\
			BEGIN\
	  		SELECT p_in, p_out, p_inout;\
			SET p_in=100, p_out=200, p_inout=300;\
			SELECT p_in, p_out, p_inout;\
			END\
			";
	int re = mysql_query(&mysql,sql.c_str());
	if (re != 0) 
	{
		cout << "mysql_query failed! " << mysql_error(&mysql) << endl;
	}
	
	//2 定义变量并赋值
	cout << "IN A=1 B=2 C=3" << endl;
	sql = "SET @A=1;SET @B=2;SET @C=3;";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed! " << mysql_error(&mysql) << endl;
	}
	do
	{
		cout << "SET affect" << mysql_affected_rows(&mysql) << endl;

	}
	// 0 还有结果 -1 没有结果 > =1 错误
	while (mysql_next_result(&mysql) == 0);
	//3 调用存储过程 call
	sql = "call p_test(@A,@B,@C)";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed! " << mysql_error(&mysql) << endl;
	}
	cout << "In Proc"<< endl;
	do
	{
		MYSQL_RES* res = mysql_store_result(&mysql);
		if (!res)
			continue;
		//打印字段数
		int fcount = mysql_num_fields(res);
		//打印结果集
		for (;;) {
			//提取一行记录
			MYSQL_ROW row = mysql_fetch_row(res);
			if (!row)break;
			for (int i = 0; i < fcount; i++)
			{
				if (row[i])
				{
					cout << row[i] << " ";
				}
				else cout << "NULL ";
			}
			cout << endl;
		}
		mysql_free_result(res);
	}
	// 0 还有结果 -1 没有结果 > =1 错误
	while (mysql_next_result(&mysql) == 0);
	//4 获取存储过程的结果
	sql = "SELECT @A,@B,@C";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed! " << mysql_error(&mysql) << endl;
	}

	MYSQL_RES* res = mysql_store_result(&mysql);
	cout << "out: ";
	MYSQL_ROW  row = mysql_fetch_row(res);
	cout << " in= " << row[0];
	cout << " out= " << row[1];
	cout << " in_out= " << row[2];
	cout << endl;
	mysql_free_result(res);
	mysql_close(&mysql);
	mysql_library_end();
	std::cout << "MySQL8.0 API!\n";


	//system("pause");
	return 0;
}