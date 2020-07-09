/************************FILE INFOMATION***********************************
**
** Project Project       : C++Mysql8.0���ݿ��ƽ̨���ʵս
** Contact               : 913902738@qq.com
** Author                : ����Ȫ
** Function              �����Զ������ļ���д
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
	//1 �����ô�Ŷ��������ݵı� t_data
	sql = "create table if not exists `t_data`(\
		`id` int AUTO_INCREMENT,\
		`name` varchar(1024),\
		`data` blob,\
		`size` int,\
		 PRIMARY KEY(`id`)\
		)";
	int re = mysql_query(&mysql,sql.c_str());
	if (re != 0)
	{
		cout << "mysql_query failed! " << mysql_error(&mysql) << endl;
	}
	//2 ��ձ� 
	sql = "truncate t_data";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cerr << "mysql_query failed! " << mysql_error(&mysql) << endl;
	}
	//3 ��ʼ��stmt mysql_stmt_init
	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
	if (!stmt)
	{
		cerr << "mysql_stmt_init failed! " << mysql_error(&mysql) << endl;
	}
	//4 Ԥ����sql���
	sql = "insert into `t_data` (name,data,size) values(?,?,?)";
	if (mysql_stmt_prepare(stmt,sql.c_str(),sql.size()))
	{
		cerr << "mysql_stmt_prepare failed! " << mysql_stmt_error(stmt) << endl;
	}

	//5 �򿪲���ȡ�ļ�
	string filename = "thz.jpg";
	//�ļ���С�Ͷ����Ƶ�ַ
	int filesize = 0;
	//��ȡ������
	fstream in(filename, ios::in | ios::binary);
	if (!in.is_open())
	{
		cerr << "file "<<filename<<" failed! "  << endl;
	}
	//�ļ�ָ���Ƶ���β��
	in.seekg(0, ios::end);
	//�ļ���С���ļ������Ƶ�ַ
	filesize = in.tellg();
	//�ص���ͷ
	in.seekg(0, ios::beg);
	char* data = new char[filesize];
	int readed = 0;//�Ѿ����˶���
	while (!in.eof())
	{
		in.read(data+readed, filesize-readed);//��ȡ���ָ��ı仯
		//���˶����ֽ�
		if (in.gcount() <= 0)
		{
			break;
		}
		readed += in.gcount();
	}
	in.close();
	

	//���ֶ�
	MYSQL_BIND bind[3] = { 0 };
	bind[0].buffer_type = MYSQL_TYPE_STRING;//name�ļ���
	bind[0].buffer = (char*)filename.c_str();
	bind[0].buffer_length = filename.size();

	bind[1].buffer_type = MYSQL_TYPE_BLOB;//data �ļ�����������
	bind[1].buffer = data;
	bind[1].buffer_length = filesize; 

	//�ļ���С
	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer = &filesize;

	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		cerr << "mysql_stmt_bind_param failed!" << mysql_stmt_error(stmt) << endl;
	}
	//7 ִ��stmt sql
	if (mysql_stmt_execute(stmt) != 0)
	{
		cerr << "mysql_stmt_execute failed!" << mysql_stmt_error(stmt) << endl;
	}

	delete []data;
	mysql_stmt_close(stmt);


	//��ѯ�����ƣ�������Ϊ�ļ�
	sql = "select * from t_data";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cerr<< "mysql_query failed! " << mysql_error(&mysql) << endl;
	}
	//��ȡ�����
	MYSQL_RES* res = mysql_store_result(&mysql);
	if (res != 0)
	{
		cerr << " mysql_store_result failed! " << mysql_error(&mysql) << endl;
	}
	MYSQL_ROW row = mysql_fetch_row(res);//char ** MYSQL_ROW array of strings
	if (!row)
	{
		cerr << " mysql_fetch_row failed! " << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << row[0] << " " << row[1] << " " << row[3] << endl;
		//��ȡÿ�����ݴ�С
		unsigned long* lens = mysql_fetch_lengths(res);
		int fnum = mysql_num_fields(res);
		for (int i = 0; i < fnum; i++)
		{
			cout << "[" << lens[i] << "]";
		}
		filename = "out_";
		filename += row[1];
		fstream out(filename, ios::out | ios::binary);
		if (out.is_open())
		{
			cerr << "open file " << filename << " failed!" << endl;
		}
		out.write(row[2], lens[2]);
		out.close();
	}

	mysql_close(&mysql);
	mysql_library_end();
	std::cout << "MySQL8.0 API!\n";
	//system("pause");
	return 0;
}