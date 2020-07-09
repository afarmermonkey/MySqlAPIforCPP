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

	//֧�ֶ���sql���
	if(!mysql_real_connect(&mysql, host, user, passwd, db, 3306, 0, CLIENT_MULTI_STATEMENTS))
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
		);";

	//������ݣ����ָ�id��1����
	sql += "truncate t_image;";//delete from t_image where id=1
	//2�������� CLIENT_MULTI_STATEMENTS
	for (int i = 0; i < 100; i++)
	{
		//sql = "insert `t_image`(`name`,`path`,`size`) values('test.jpg','d:/img/test.jpg',10240)";
		stringstream ss;
		ss << "insert `t_image`(`name`,`path`,`size`) values('image";
		ss << i << ".jpg'" << ",'d:/img',10240);";
		sql += ss.str();		
	}
	

	//3�޸�����
	//UPDATE `zwq`.`t_image` SET `size` = '3000' WHERE (`id` = '3031');
	sql += "UPDATE `t_image` SET `name` = 'test2' WHERE (`id` = '2');";
	

	//4ɾ������
	//delete����ɾ���ռ䣬ֻ���˱�ʶ�����ݲ����ˣ����ڴ滹ռ�ã�
	sql += "delete from	`zwq`.`t_image` where `id`='1';";//����ѡ��ɾ������delete from t_image where id=1
	

	//����
	sql += "select * from t_image;";
	//ִ��sql��䣬�������أ�����䲢û��ȫ��ִ�кţ���Ҫ��ȡ���
	//ֻ�ǰ�sql��䷢��MySQL server�� ɫ����һ����ִ�У����ؽ��
	int re = mysql_query(&mysql,sql.c_str());
	if (re != 0)
	{
		cout << "mysql query failed!" << mysql_error(&mysql) << endl;
	}
	//�ж�����ؽ��
	do
	{
		cout << "[result]";
		MYSQL_RES* result = mysql_store_result(&mysql);
		if(result) //�н���� select
		{
			cout << "Select mysql_num_rows	=" << mysql_num_rows(result) << endl;
			mysql_free_result(result);
		}
		else //û�н����  insert update delete create tlabe drop truncate
		{
			//���ֶ� �޽��  select����   �ٷ��д�����
			if (mysql_field_count(&mysql) > 0)
			{
				cout << "mysql_num_rows	=" << mysql_num_rows(result) << endl;
				cout << "Not retrieve result!" << mysql_error(&mysql) << endl;
			}
			else //û�н����  insert update delete create tlabe drop truncate
			{
				//�ȴ�������������
				cout << "mysql affected rows " << mysql_affected_rows(&mysql) << endl;
			}
		}
	}//ȡ��һ�����
	while(mysql_next_result(&mysql)==0);
	mysql_library_end();
	std::cout << "MySQL8.0 API!\n";
	//system("pause");
	return 0;
}