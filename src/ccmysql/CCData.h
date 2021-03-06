/************************FILE INFOMATION***********************************
**
** Project Project       : C++Mysql8.0数据库跨平台编程实战
** Contact               : 913902738@qq.com
** Author                : 朱文泉
** CCData  ：数据库封装所用的数据结构
***************************************************************************/
#ifndef _CCDATA_H
#define _CCDATA_H
#ifdef _WIN32
	#ifdef MYSQL_EXPORTS
	//动态链接库调用
	#define MYAPI __declspec(dllexport)
	#else
	//执行程序调用
	#define MYAPI __declspec(dllimport)
	#endif
#else  //linux mac
	#define MYAPI 
#endif // _WIN32
#include<map>
#include<vector>
#include<string>
namespace cc {
	enum CC_OPT {
		CC_MYSQL_OPT_CONNECT_TIMEOUT,
		CC_MYSQL_OPT_COMPRESS,
		CC_MYSQL_OPT_NAMED_PIPE,
		CC_MYSQL_INIT_COMMAND,
		CC_MYSQL_READ_DEFAULT_FILE,
		CC_MYSQL_READ_DEFAULT_GROUP,
		CC_MYSQL_SET_CHARSET_DIR,
		CC_MYSQL_SET_CHARSET_NAME,
		CC_MYSQL_OPT_LOCAL_INFILE,
		CC_MYSQL_OPT_PROTOCOL,
		CC_MYSQL_SHARED_MEMORY_BASE_NAME,
		CC_MYSQL_OPT_READ_TIMEOUT,
		CC_MYSQL_OPT_WRITE_TIMEOUT,
		CC_MYSQL_OPT_USE_RESULT,
		CC_MYSQL_REPORT_DATA_TRUNCATION,
		CC_MYSQL_OPT_RECONNECT,
		CC_MYSQL_PLUGIN_DIR,
		CC_MYSQL_DEFAULT_AUTH,
		CC_MYSQL_OPT_BIND,
		CC_MYSQL_OPT_SSL_KEY,
		CC_MYSQL_OPT_SSL_CERT,
		CC_MYSQL_OPT_SSL_CA,
		CC_MYSQL_OPT_SSL_CAPATH,
		CC_MYSQL_OPT_SSL_CIPHER,
		CC_MYSQL_OPT_SSL_CRL,
		CC_MYSQL_OPT_SSL_CRLPATH,
		CC_MYSQL_OPT_CONNECT_ATTR_RESET,
		CC_MYSQL_OPT_CONNECT_ATTR_ADD,
		CC_MYSQL_OPT_CONNECT_ATTR_DELETE,
		CC_MYSQL_SERVER_PUBLIC_KEY,
		CC_MYSQL_ENABLE_CLEARTEXT_PLUGIN,
		CC_MYSQL_OPT_CAN_HANDLE_EXPIRED_PASSWORDS,
		CC_MYSQL_OPT_MAX_ALLOWED_PACKET,
		CC_MYSQL_OPT_NET_BUFFER_LENGTH,
		CC_MYSQL_OPT_TLS_VERSION,
		CC_MYSQL_OPT_SSL_MODE,
		CC_MYSQL_OPT_GET_SERVER_PUBLIC_KEY,
		CC_MYSQL_OPT_RETRY_COUNT,
		CC_MYSQL_OPT_OPTIONAL_RESULTSET_METADATA,
		CC_MYSQL_OPT_SSL_FIPS_MODE
	};
	enum FIELD_TYPE {
		CC_MYSQL_TYPE_DECIMAL,
		CC_MYSQL_TYPE_TINY,
		CC_MYSQL_TYPE_SHORT,
		CC_MYSQL_TYPE_LONG,
		CC_MYSQL_TYPE_FLOAT,
		CC_MYSQL_TYPE_DOUBLE,
		CC_MYSQL_TYPE_NULL,
		CC_MYSQL_TYPE_TIMESTAMP,
		CC_MYSQL_TYPE_LONGLONG,
		CC_MYSQL_TYPE_INT24,
		CC_MYSQL_TYPE_DATE,
		CC_MYSQL_TYPE_TIME,
		CC_MYSQL_TYPE_DATETIME,
		CC_MYSQL_TYPE_YEAR,
		CC_MYSQL_TYPE_NEWDATE, /**< Internal to MySQL. Not used in protocol */
		CC_MYSQL_TYPE_VARCHAR,
		CC_MYSQL_TYPE_BIT,
		CC_MYSQL_TYPE_TIMESTAMP2,
		CC_MYSQL_TYPE_DATETIME2, /**< Internal to MySQL. Not used in protocol */
		CC_MYSQL_TYPE_TIME2,     /**< Internal to MySQL. Not used in protocol */
		CC_MYSQL_TYPE_JSON = 245,
		CC_MYSQL_TYPE_NEWDECIMAL = 246,
		CC_MYSQL_TYPE_ENUM = 247,
		CC_MYSQL_TYPE_SET = 248,
		CC_MYSQL_TYPE_TINY_BLOB = 249,
		CC_MYSQL_TYPE_MEDIUM_BLOB = 250,
		CC_MYSQL_TYPE_LONG_BLOB = 251,
		CC_MYSQL_TYPE_BLOB = 252,
		CC_MYSQL_TYPE_VAR_STRING = 253,
		CC_MYSQL_TYPE_STRING = 254,
		CC_MYSQL_TYPE_GEOMETRY = 255
	} ;
	struct MYAPI CCData
	{
		CCData(const char* data=0);//字符串
		CCData(const int* d);//整形

		//二进制
		//读取文件大小，内容写入data，size大小。 会在堆中申请data空间，需要用Drop释放
		bool LoadFile(const char* filename);
		bool SaveFile(const char* filename);
		const char* data = 0;
		int size = 0;
		FIELD_TYPE type;
		//释放 LoadFile申请的空间
		void Drop();
		std::string UTF8ToGBK();
		std::string GBKToUTF8();
	};
	//插入和更新的数据结构map<字段名，数据类型>
	typedef std::map<std::string, CCData> SQLDATA;

	//数据列表
	typedef std::vector<std::vector<CCData> >XROWS;
}

#endif
