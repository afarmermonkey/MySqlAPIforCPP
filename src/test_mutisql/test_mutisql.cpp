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

	//支持多条sql语句
	if(!mysql_real_connect(&mysql, host, user, passwd, db, 3306, 0, CLIENT_MULTI_STATEMENTS))
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
		);";

	//清空数据，并恢复id从1自增
	sql += "truncate t_image;";//delete from t_image where id=1
	//2插入数据 CLIENT_MULTI_STATEMENTS
	for (int i = 0; i < 100; i++)
	{
		//sql = "insert `t_image`(`name`,`path`,`size`) values('test.jpg','d:/img/test.jpg',10240)";
		stringstream ss;
		ss << "insert `t_image`(`name`,`path`,`size`) values('image";
		ss << i << ".jpg'" << ",'d:/img',10240);";
		sql += ss.str();		
	}
	

	//3修改数据
	//UPDATE `zwq`.`t_image` SET `size` = '3000' WHERE (`id` = '3031');
	sql += "UPDATE `t_image` SET `name` = 'test2' WHERE (`id` = '2');";
	

	//4删除数据
	//delete不会删除空间，只做了标识（数据不见了，但内存还占用）
	sql += "delete from	`zwq`.`t_image` where `id`='1';";//可以选择删除条件delete from t_image where id=1
	

	//查找
	sql += "select * from t_image;";
	//执行sql语句，立即返回，但语句并没有全部执行号，需要获取结果
	//只是把sql语句发给MySQL server， 色热热一条条执行，返回结果
	int re = mysql_query(&mysql,sql.c_str());
	if (re != 0)
	{
		cout << "mysql query failed!" << mysql_error(&mysql) << endl;
	}
	//有多个返回结果
	do
	{
		cout << "[result]";
		MYSQL_RES* result = mysql_store_result(&mysql);
		if(result) //有结果集 select
		{
			cout << "Select mysql_num_rows	=" << mysql_num_rows(result) << endl;
			mysql_free_result(result);
		}
		else //没有结果集  insert update delete create tlabe drop truncate
		{
			//有字段 无结果  select出错   官方判错例子
			if (mysql_field_count(&mysql) > 0)
			{
				cout << "mysql_num_rows	=" << mysql_num_rows(result) << endl;
				cout << "Not retrieve result!" << mysql_error(&mysql) << endl;
			}
			else //没有结果集  insert update delete create tlabe drop truncate
			{
				//等待服务器处理结果
				cout << "mysql affected rows " << mysql_affected_rows(&mysql) << endl;
			}
		}
	}//取下一条结果
	while(mysql_next_result(&mysql)==0);
	mysql_library_end();
	std::cout << "MySQL8.0 API!\n";
	//system("pause");
	return 0;
}