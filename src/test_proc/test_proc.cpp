/************************FILE INFOMATION***********************************
**
** Project Project       : C++Mysql8.0���ݿ��ƽ̨���ʵս
** Contact               : 913902738@qq.com
** Author                : ����Ȫ
** Function              �����Դ洢����
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
		std::cout << "mysql connect" << host << "  success!" << std::endl;
	}
	string sql = "";
	//1 �����洢����
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
	
	//2 �����������ֵ
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
	// 0 ���н�� -1 û�н�� > =1 ����
	while (mysql_next_result(&mysql) == 0);
	//3 ���ô洢���� call
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
		//��ӡ�ֶ���
		int fcount = mysql_num_fields(res);
		//��ӡ�����
		for (;;) {
			//��ȡһ�м�¼
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
	// 0 ���н�� -1 û�н�� > =1 ����
	while (mysql_next_result(&mysql) == 0);
	//4 ��ȡ�洢���̵Ľ��
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