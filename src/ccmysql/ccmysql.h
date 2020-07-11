#ifndef _CCMYSQL_H
#define _CCMYSQL_H

#include <vector>
#include "CCData.h"

struct MYSQL;
struct MYSQL_RES;
namespace cc {	

	class MYAPI CCMysql
	{
	public:
		//��ʼ��Mysql API
		bool Init();

		//����ռ�õ�������Դ
		void Close();

		//���ݿ����ӣ��������̰߳�ȫ��flag�����Ƿ�֧�ֶ������
		bool Connect(const char* host, const char* user, const char* pass, const char* db, const unsigned short port = 3306, unsigned long flag = 0);
		
		//ִ��sql��� if sqllen = 0 strlen ��ȡ�ַ�����
		bool Query(const char* sql, unsigned long sqllen = 0);

		//Mysql�������趨
		bool Options(CC_OPT opt, const void* arg);

		//���ӳ�ʱ
		bool SetConnectTionout(int sec);

		//�Զ�����,Ĭ�ϲ��Զ�
		bool SetReconnect(bool isre = true);

		//��ȡ�����
		//StoreRsult() ����ȫ�����
		//UseResult()  ��ʼ���գ�ͨ��Fetch��ȡ
		bool StoreResult();
		bool UseResult();

		//�ͷŽ����ռ�ÿռ�
		void FreeResult();

		//��ȡһ������
		std::vector<CCData> FetchRow();

		//����insert sql���
		std::string GetInsertSql(SQLDATA kv,std::string table);

		//����Ƕ���������
		//�ֶ�����ǰ��@ ����@time �������ݲ������ţ�һ������MySQL�Ĺ��ܺ�������
		bool Insert(SQLDATA kv, std::string table);
		//�������������
		bool InsertBin(SQLDATA kv, std::string table);
		
		//��ȡ�������ݵ�sql��� where ����� �û�Ҫ����where
		std::string GetUpdateSql(SQLDATA kv, std::string table,std::string where);
		//�������ظ���������ʧ�ܷ���-1
		int Update(SQLDATA kv, std::string table, std::string where);
		int UpdateBin(SQLDATA kv, std::string table, std::string where);

		//����ӿ�
		bool StartTransaction();//��ʼ����  �ر��Զ��ύ
		bool StopTransaction();//ֹͣ����   �����Զ��ύ
		bool Commit();//�ύ
		bool RollBack();//�ع�

		//���׽ӿ� ����select���ݽ����ÿ���Զ�����������һ�ν����
		XROWS GetResult(const char* sql);
	protected:
		MYSQL* mysql = 0;
		MYSQL_RES* result=0;
	public:
		CCMysql();
		~CCMysql();
	private:
	};

	
}

#endif