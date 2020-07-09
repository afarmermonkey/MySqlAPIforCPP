#include<iostream>
#include<mysql.h>
#include<thread>
using namespace std;
int main()
{
	MYSQL mysql;
	mysql_init(&mysql);//初始化MySQL
	//mysql_library_init();
	const char* host = "192.168.37.130";
	const char* user = "root";
	const char* passwd = "654321";
	const char* db = "zwq";//数据库名  use mysql

	if(!mysql_real_connect(&mysql, host, user, passwd, db, 3306, 0, 0))
	{
		std::cout << "mysql connect failed" << mysql_error(&mysql)<<std::endl;
	}
	else
	{
		std::cout << "mysql connect" << host << "success!" << std::endl;
	}
	//查找user表 select * from user
	//第一步，执行sql语句 （必须完成三步！）
	const char* sql = "select * from t_image";
	//两种执行sql语句方法 
	//mysql_real_query sql语句中可以包含二进制数据
	//mysql_query sql语句中只能是字符串
	//返回0表示成功，错误返回错误编码
	int re = mysql_real_query(&mysql, sql, strlen(sql));  //select * from user
	//Commands out of sync; you can't run this command now
	//执行sql语句后，必须完成结果获取（第二步）并且清理（第三步）
	if (re != 0)
	{
		cout << "mysql_real_query failed!" << sql << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << "mysql_real_query success!" << sql << endl;
	}
	//第二步获取结果集
	//mysql_use_result 开始读取结果集 不实际读取数据 (第三步 遍历和清理时读取)
	//mysql_store_result 读取全部数据（注意缓存大小，MYSQL_OPT_MAX_ALLOWED_PACKET 默认64MB）
	//MYSQL_RES* result = mysql_use_result(&mysql);
	MYSQL_RES* result = mysql_store_result(&mysql);
	if (!result)
	{
		cout << "mysql_use_result failed!" << mysql_error(&mysql) << endl;;
	}
	//获取表字段
	MYSQL_FIELD* field;
	while (field=mysql_fetch_field(result))
	{
		cout << "key:" << field->name << endl;
	}
	//获取表字段数量
	int fnum = mysql_num_fields(result);
	//第三步遍历结果集
	MYSQL_ROW row;
	while (row = mysql_fetch_row(result))
	{
		unsigned long *lens = mysql_fetch_lengths(result);

		//cout <<lens[0]<< "[" << row[0] << "]" << "," <<lens[1]<< "[" << row[1] << "]" << endl;
		for (int i = 0; i <fnum; i++)
		{
			cout << mysql_fetch_field_direct(result, i)->name << ": ";
			if(row[i])
				cout << row[i];
			else
				cout << "null";
			cout<< ","<<endl;
		}
		cout <<"\n==========================================" <<endl;
	}
	//清理结果集]
	mysql_free_result(result);
	mysql_close(&mysql);
	mysql_library_end();
	std::cout << "Hello Word!\n";
	//system("pause");
	return 0;
}