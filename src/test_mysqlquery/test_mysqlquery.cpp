#include<iostream>
#include<mysql.h>
#include<thread>
using namespace std;
int main()
{
	MYSQL mysql;
	mysql_init(&mysql);//��ʼ��MySQL
	//mysql_library_init();
	const char* host = "192.168.37.130";
	const char* user = "root";
	const char* passwd = "654321";
	const char* db = "zwq";//���ݿ���  use mysql

	if(!mysql_real_connect(&mysql, host, user, passwd, db, 3306, 0, 0))
	{
		std::cout << "mysql connect failed" << mysql_error(&mysql)<<std::endl;
	}
	else
	{
		std::cout << "mysql connect" << host << "success!" << std::endl;
	}
	//����user�� select * from user
	//��һ����ִ��sql��� �����������������
	const char* sql = "select * from t_image";
	//����ִ��sql��䷽�� 
	//mysql_real_query sql����п��԰�������������
	//mysql_query sql�����ֻ�����ַ���
	//����0��ʾ�ɹ������󷵻ش������
	int re = mysql_real_query(&mysql, sql, strlen(sql));  //select * from user
	//Commands out of sync; you can't run this command now
	//ִ��sql���󣬱�����ɽ����ȡ���ڶ���������������������
	if (re != 0)
	{
		cout << "mysql_real_query failed!" << sql << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << "mysql_real_query success!" << sql << endl;
	}
	//�ڶ�����ȡ�����
	//mysql_use_result ��ʼ��ȡ����� ��ʵ�ʶ�ȡ���� (������ ����������ʱ��ȡ)
	//mysql_store_result ��ȡȫ�����ݣ�ע�⻺���С��MYSQL_OPT_MAX_ALLOWED_PACKET Ĭ��64MB��
	//MYSQL_RES* result = mysql_use_result(&mysql);
	MYSQL_RES* result = mysql_store_result(&mysql);
	if (!result)
	{
		cout << "mysql_use_result failed!" << mysql_error(&mysql) << endl;;
	}
	//��ȡ���ֶ�
	MYSQL_FIELD* field;
	while (field=mysql_fetch_field(result))
	{
		cout << "key:" << field->name << endl;
	}
	//��ȡ���ֶ�����
	int fnum = mysql_num_fields(result);
	//���������������
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
	//��������]
	mysql_free_result(result);
	mysql_close(&mysql);
	mysql_library_end();
	std::cout << "Hello Word!\n";
	//system("pause");
	return 0;
}