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
	mysql_init(&mysql);//��ʼ��MySQL

	const char* host = "192.168.37.130";
	const char* user = "root";
	const char* passwd = "654321";
	const char* db = "zwq";//���ݿ���

	if(!mysql_real_connect(&mysql, host, user, passwd, db, 3306, 0, 0))
	{
		std::cout << "mysql connect failed" << mysql_error(&mysql)<<std::endl;
	}
	else
	{
		std::cout << "mysql connect" << host << "success!" << std::endl;
	}
	//1������
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
	//2�������� CLIENT_MULTI_STATEMENTS
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
	

	//3�޸�����
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


	//����map�Զ�����sql���
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
	temp += "id=id";//��һ���Ǳ���temp�������Ƕ��ţ�Ҳ����ɾ�����һ������
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

	//4ɾ������
	//delete����ɾ���ռ䣬ֻ���˱�ʶ�����ݲ����ˣ����ڴ滹ռ�ã�
	sql = "delete from t_image where id=1";//����ѡ��ɾ������delete from t_image where id=1
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
	//ʵ�������˿ռ�
	sql = "OPTIMIZE TABLE t_image where id=1";
	re = mysql_query(&mysql, sql.c_str());

	//������ݣ����ָ�id��1����
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