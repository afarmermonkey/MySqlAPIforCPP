/************************FILE INFOMATION***********************************
**
** Project Project       : C++Mysql8.0���ݿ��ƽ̨���ʵս
** Contact               : 913902738@qq.com
** Author                : ����Ȫ
** Function              �����ԶϿ�����
***************************************************************************/

#include<iostream>
#include<mysql.h>
#include<thread>
using namespace std;
int main()
{
	MYSQL mysql;
	mysql_init(&mysql);
	
	const char* host = "192.168.37.130";
	const char* user = "root";
	const char* passwd = "654321";
	const char* db = "mysql";//���ݿ���

   //�趨��ʱ3s
	int to = 3;
	int re = mysql_options(&mysql, MYSQL_OPT_CONNECT_TIMEOUT, &to);
	if (re != 0)
	{
		cout << "mysql_options failed" << mysql_error(&mysql) << endl;
	}
	//�Զ�����
	int recon = 1;
	re = mysql_options(&mysql, MYSQL_OPT_RECONNECT, &recon);
	if (re != 0)
	{
		cout << "mysql_options failed" << mysql_error(&mysql) << endl;
	}
	if(!mysql_real_connect(&mysql, host, user, passwd, db, 3306, 0, 0))
	{
		std::cout << "mysql connect failed" << mysql_error(&mysql)<<std::endl;
	}
	else
	{
		std::cout << "mysql connect" << host << "success!" << std::endl;
	}
	for (int i=0;i<1000;i++)
	{
		int re = mysql_ping(&mysql);
		if (re == 0)
		{
			cout << host << ":mysql ping success!" << endl;
		}
		else
		{
			cout << host << ":mysql ping failed!" << mysql_error(&mysql) << endl;
		}
		this_thread::sleep_for(1s);
	}
	std::cout << "Hello Word!\n";
	//system("pause");
	return 0;
}