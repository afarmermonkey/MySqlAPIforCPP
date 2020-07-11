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
		//初始化Mysql API
		bool Init();

		//清理占用的所有资源
		void Close();

		//数据库链接（不考虑线程安全）flag设置是否支持多条语句
		bool Connect(const char* host, const char* user, const char* pass, const char* db, const unsigned short port = 3306, unsigned long flag = 0);
		
		//执行sql语句 if sqllen = 0 strlen 获取字符长度
		bool Query(const char* sql, unsigned long sqllen = 0);

		//Mysql参数的设定
		bool Options(CC_OPT opt, const void* arg);

		//连接超时
		bool SetConnectTionout(int sec);

		//自动重连,默认不自动
		bool SetReconnect(bool isre = true);

		//获取结果集
		//StoreRsult() 返回全部结果
		//UseResult()  开始接收，通过Fetch获取
		bool StoreResult();
		bool UseResult();

		//释放结果集占用空间
		void FreeResult();

		//获取一行数据
		std::vector<CCData> FetchRow();

		//生成insert sql语句
		std::string GetInsertSql(SQLDATA kv,std::string table);

		//插入非二进制数据
		//字段名称前加@ 比如@time ，其内容不加引号，一般用于MySQL的功能函数调用
		bool Insert(SQLDATA kv, std::string table);
		//插入二进制数据
		bool InsertBin(SQLDATA kv, std::string table);
		
		//获取更新数据的sql语句 where 语句中 用户要包含where
		std::string GetUpdateSql(SQLDATA kv, std::string table,std::string where);
		//函数返回更新数量，失败返回-1
		int Update(SQLDATA kv, std::string table, std::string where);
		int UpdateBin(SQLDATA kv, std::string table, std::string where);

		//事务接口
		bool StartTransaction();//开始事务  关闭自动提交
		bool StopTransaction();//停止事务   启动自动提交
		bool Commit();//提交
		bool RollBack();//回滚

		//简易接口 返回select数据结果，每次自动调用清理上一次结果集
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