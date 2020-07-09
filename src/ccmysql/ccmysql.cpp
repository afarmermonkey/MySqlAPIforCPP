/************************FILE INFOMATION***********************************
**
** Project Project       : C++Mysql8.0���ݿ��ƽ̨���ʵս
** Contact               : 913902738@qq.com
** Author                : ����Ȫ
** CCMysql  ����װ��mysql�࣬���е����ݿ�����������Ա����
***************************************************************************/


#include"ccmysql.h"
#include<iostream>
#include<mysql.h>
#include<string.h>
using namespace std;
//���к��������ܱ�֤�̰߳�ȫ
namespace cc {

	bool CCMysql::Init() //��ʼ��
	{

		//�´���һ��MYSQL ����
		mysql = mysql_init(0);
		if (!mysql)
		{
			cerr << "mysql_init() failed��" << endl;
			return false;
		}
		return true;
	}

	//����ռ�õ�������Դ
	void CCMysql::Close()
	{
		FreeResult();
		if (mysql)
		{
			mysql_close(mysql);
			mysql = NULL;
		}
		cout << "CCMysql::Close()!" << endl;
	}
	//���ݿ����ӣ��������̰߳�ȫ��flag�����Ƿ�֧�ֶ������
	bool CCMysql::Connect(const char* host, const char* user, const char* pass, const char* db, const unsigned short port, unsigned long flag)
	{

		if (!mysql && !Init())
		{
			cerr << "Mysql connect failed! mysql is not init!" << endl;
			return false;
		}
		if (!mysql_real_connect(mysql, host, user, pass, db, port, 0, flag))
		{
			cerr << "Mysql connect failed!" << mysql_error(mysql) << endl;
			return false;
		}
		return true;
	}
	//mysql ���ִ��
	bool CCMysql::Query(const char* sql, unsigned long sqllen)
	{

		if (!mysql)
		{
			cerr << "Query failed!: mysql is NULL	" << endl;
			return false;
		}
		if (!sql)
		{
			cerr << "Query failed!: sql is NULL" << endl;
			return false;
		}
		if (sqllen <= 0)
		{
			sqllen = strlen(sql);
		}
		if (sqllen <= 0)
		{
			cerr << "Query sql is empty or wrong format!" << endl;
		}
		int re = mysql_real_query(mysql, sql, sqllen);
		if (re != 0)
		{
			cerr << "mysql_real_query failed!" << mysql_error(mysql) << endl;
			return false;
		}
		return true;
	}
	//mysql �������� Connect֮ǰ����
	bool CCMysql::Options(CC_OPT opt, const void* arg)
	{
		if (!mysql)
		{
			cerr << "Option failed!: mysql is NULL	" << endl;
			return false;
		}
		int re = mysql_options(mysql, (mysql_option)opt, arg);
		if (re != 0)
		{
			cerr << "mysql_options failed!" << mysql_error(mysql) << endl;
			return false;
		}
		return true;
	}

	//���ӳ�ʱ
	bool CCMysql::SetConnectTionout(int sec)
	{
		return Options(CC_MYSQL_OPT_CONNECT_TIMEOUT, &sec);
	}
	//�Զ�����,Ĭ�ϲ��Զ�
	bool CCMysql::SetReconnect(bool isre)
	{
		return Options(CC_MYSQL_OPT_RECONNECT, &isre);

	}

	//��ȡ�����
	//StoreRsult() ����ȫ�����
	//UseResult()  ��ʼ���գ�ͨ��Fetch��ȡ
	bool CCMysql::StoreResult()
	{
		if (!mysql)
		{
			cerr << "StoreRsult failed!: mysql is NULL	" << endl;
			return false;
		}
		FreeResult();
		result = mysql_store_result(mysql);
		if (!result)
		{
			cerr << "mysql_store_result failed!: result is NULL	" << mysql_error(mysql) << endl;
			return false;
		}
		return true;
	}
	bool CCMysql::UseResult()
	{
		if (!mysql)
		{
			cerr << "UseResult failed!: mysql is NULL	" << endl;
			return false;
		}
		FreeResult();
		result = mysql_use_result(mysql);
		if (!result)
		{
			cerr << "mysql_use_result failed!: result is NULL	" << mysql_error(mysql) << endl;
			return false;
		}
		return true;
	}
	//�ͷŽ����ռ�ÿռ�
	void CCMysql::FreeResult()
	{
		if (result)
		{
			mysql_free_result(result);
			result = NULL;
		}
	}
	//��ȡһ������
	std::vector<CCData> CCMysql::FetchRow()
	{
		std::vector<CCData> re;
		if (!result)
		{
			return re;
		}
		MYSQL_ROW row = mysql_fetch_row(result);
		if (!row)
		{
			return re;
		}
		//����
		int num = mysql_num_fields(result);
		unsigned long* lens = mysql_fetch_lengths(result);
		for (int i = 0; i < num; i++)
		{
			CCData data;
			data.data = row[i];
			data.size = lens[i];
			//��ȡ�е�����
			auto field=mysql_fetch_field_direct(result,i);
			data.type = (FIELD_TYPE)field->type;
			re.push_back(data);
		}
		return re;
	}
	//����insert sql���
	std::string CCMysql::GetInsertSql(SQLDATA kv, std::string table)
	{
		string sql = "";
		if (kv.empty() || table.empty())
			return "";
		sql = " insert into `";
		sql += table;
		sql += "`";
		//insert into t_ccmysql (name,size) vlause("name1","1024")
		string keys = "";
		string vals = "";
		//����map
		for (auto ptr = kv.begin(); ptr != kv.end(); ptr++)
		{
			keys += "`";
			keys += ptr->first;
			keys += "`,";
			vals += "'";
			vals += ptr->second.data;
			vals += "',";
		}
		//ȥ����β����Ķ���
		keys[keys.size() - 1] = ' ';
		vals[vals.size() - 1] = ' ';

		sql += "(";
		sql += keys;
		sql += ")values(";
		sql += vals;
		sql += ")";
		
		return sql;
	}

	bool CCMysql::Insert(SQLDATA kv, std::string table)
	{
		if (!mysql)
		{
			cerr << "Insert failed!: mysql is NULL	" << endl;
			return false;
		}
		string sql = GetInsertSql(kv, table);
		if (sql.empty())
			return false;
		if (!Query(sql.c_str()))
			return false;
		int num = mysql_affected_rows(mysql);
		if (num <= 0)
			return false;
		return true;
	}
	//����Ƕ���������
	bool CCMysql::InsertBin(SQLDATA kv, std::string table)
	{
		string sql = "";
		if (kv.empty() || table.empty()||!mysql)
			return false;
		sql = " insert into `";
		sql += table;
		sql += "`";
		//insert into t_ccmysql (name,size) vlause(?,?)
		string keys = "";
		string vals = "";
		//���ֶ�
		//MYSQL_BIND *bind=new MYSQL_BIND[kv.size()*2];
		MYSQL_BIND bind[100] = {0};
		int i = 0;
		//����map
		for (auto ptr = kv.begin(); ptr != kv.end(); ptr++)
		{
			keys += "`";
			keys += ptr->first;
			keys += "`,";
			vals += "?,";
			bind[i].buffer=(char*)ptr->second.data;
			bind[i].buffer_length = ptr->second.size;
			bind[i].buffer_type = (enum_field_types)ptr->second.type;
			i++;
		}
		//ȥ����β����Ķ���
		keys[keys.size() - 1] = ' ';
		vals[vals.size() - 1] = ' ';
		sql += "(";
		sql += keys;
		sql += ")values(";
		sql += vals;
		sql += ")";
		//Ԥ����SQL���
		MYSQL_STMT* stmt = mysql_stmt_init(mysql);
		if (!stmt)
		{
			cerr << "mysql_stmt_init failed!" << mysql_error(mysql) << endl;
			return false;
		}
		if (mysql_stmt_prepare(stmt, sql.c_str(), sql.length()) != 0)
		{
			
			cerr << "mysql_stmt_prepare failed!" << mysql_stmt_error(stmt) << endl;
			mysql_stmt_close(stmt);
			return false;
		}

		if (mysql_stmt_bind_param(stmt, bind) != 0)
		{
			
			cerr << "mysql_stmt_bind_param failed!" << mysql_stmt_error(stmt) << endl;
			mysql_stmt_close(stmt);
			return false;
		}
		if (mysql_stmt_execute(stmt) != 0)
		{
			
			cerr << "mysql_stmt_execute failed!" << mysql_stmt_error(stmt) << endl;
			mysql_stmt_close(stmt);
			return false;
		}
		mysql_stmt_close(stmt);
		return true;
	}
	//��ȡ�������ݵ�sql��� where ����� �û�Ҫ����where
	std::string CCMysql::GetUpdateSql(SQLDATA kv, std::string table,std::string where)
	{
		string sql;
		if (kv.empty() || table.empty())
			return "";
		sql = " update `";
		sql += table;
		sql += "` set ";
		//update t_ccmysql set name='update001',size=100,where id=10
		//����map
		for (auto ptr = kv.begin(); ptr != kv.end(); ptr++)
		{
			sql += "`";
			sql += ptr->first;
			sql += "`='";
			sql += ptr->second.data;
			sql += "',";
		}
		//ȥ����β����Ķ���
		sql[sql.size() - 1] = ' ';
		sql += " ";
		sql += where;
		return sql;
	}
	int CCMysql::Update(SQLDATA kv, std::string table, std::string where)
	{
		if (!mysql)return -1;
		string sql = GetUpdateSql(kv, table, where);
		if (sql.empty())return -1;
		if (!Query(sql.c_str()))
		{
			return -1;
		}
		return mysql_affected_rows(mysql);
	}

	int CCMysql::UpdateBin(SQLDATA kv, std::string table, std::string where)
	{
		if (!mysql || kv.empty() || table.empty())
		{
			return -1;
		}
		string sql = "";
		sql = " update `";
		sql += table;
		sql += "` set ";
		//update t_ccmysql set name='update001',size=100,where id=10
		
		//����map
		MYSQL_BIND bind[100] = { 0 };
		int i = 0;

		for (auto ptr = kv.begin(); ptr != kv.end(); ptr++)
		{
			sql += "`";
			sql += ptr->first;
			sql += "`=?,";
			bind[i].buffer = (char*)ptr->second.data;
			bind[i].buffer_length = ptr->second.size;
			bind[i].buffer_type = (enum_field_types)ptr->second.type;
			i++;
		}
		//ȥ����β����Ķ���
		sql[sql.size() - 1] = ' ';
		sql += " ";
		sql += where;

		//Ԥ����SQL���������
		MYSQL_STMT* stmt = mysql_stmt_init(mysql);
		if (!stmt)
		{
			cerr << "mysql_stmt_init failed!" << mysql_error(mysql) << endl;
			return -1;
		}
		if (mysql_stmt_prepare(stmt, sql.c_str(), sql.length()) != 0)
		{

			cerr << "mysql_stmt_prepare failed!" << mysql_stmt_error(stmt) << endl;
			mysql_stmt_close(stmt);
			return -1;
		}

		if (mysql_stmt_bind_param(stmt, bind) != 0)
		{

			cerr << "mysql_stmt_bind_param failed!" << mysql_stmt_error(stmt) << endl;
			mysql_stmt_close(stmt);
			return -1;
		}
		if (mysql_stmt_execute(stmt) != 0)
		{

			cerr << "mysql_stmt_execute failed!" << mysql_stmt_error(stmt) << endl;
			mysql_stmt_close(stmt);
			return -1;
		}
		mysql_stmt_close(stmt);
		return mysql_stmt_affected_rows(stmt);
	}

	//����ӿ�
	bool CCMysql::StartTransaction()
	{
		return Query("set autocommit =0");
	}
	bool CCMysql::StopTransaction()
	{
		return Query("set autocommit =1");
	}
	bool CCMysql::Commit()
	{
		return Query("commit");
	}
	bool CCMysql::RollBack()
	{
		return Query("rollback");
	}

	//���׽ӿ� ����select���ݽ����ÿ���Զ�����������һ�ν����
	XROWS CCMysql::GetResult(const char* sql) 
	{
		FreeResult();
		XROWS rows;
		if (!Query(sql))
			return rows;
		if (!StoreResult())
			return rows;
		for (;;)
		{
			auto row = FetchRow();
			if (row.empty())break;
			rows.push_back(row);
		}
		return rows;
	}


}