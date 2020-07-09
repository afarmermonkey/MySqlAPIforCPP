/************************FILE INFOMATION***********************************
**
** Project Project       : C++Mysql8.0数据库跨平台编程实战
** Contact               : 913902738@qq.com
** Author                : 朱文泉
** Function              ：测试字符集转换
***************************************************************************/

#include <iostream>
#include<string>
#ifdef _WIN32
#include<Windows.h>
#endif

using namespace std;
string UTFToGBK(const char* data)
{
	string utf8 = "";
	//1 UFT8 转为unicode win utf16
	#ifdef _WIN32
	//1.1 统计转换后字节数
	int len = MultiByteToWideChar(CP_UTF8,	//转换的格式
		0,			//默认的转换方式
		data,		//输入的字节
		-1,			//输入的字符串大小 -1 找0
		0,//输出
		0//输出的空间大小
	);
	if (len <= 0)
		return utf8;
	wstring udata;
	udata.resize(len);
	MultiByteToWideChar(CP_UTF8, 0, data, -1, (wchar_t*)udata.data(), len);

	//2 unicode 转GBK
	len = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, 0, 0,
		0,	//失败默认替代字符
		0	//是否使用默认替代 
	);
	if (len <= 0)
		return utf8;
	utf8.resize(len);
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, (char*)utf8.data(), len, 0, 0);
	#endif
	return utf8;
}
string GBKToUTF8(const char* data)
{
	string gbk = "";
	//GBK转unicode
	//1.1 统计转换后字节数
	#ifdef _WIN32
	int len = MultiByteToWideChar(CP_ACP,	//转换的格式
		0,			//默认的转换方式
		data,		//输入的字节
		-1,			//输入的字符串大小 -1 找\0
		0,//输出
		0//输出的空间大小
	);
	if (len <= 0)
		return gbk;
	wstring udata;
	udata.resize(len);
	MultiByteToWideChar(CP_ACP, 0, data, -1, (wchar_t*)udata.data(), len);

	//2 unicode 转utf-8
	len = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, 0, 0,
		0,	//失败默认替代字符
		0	//是否使用默认替代 
	);
	if (len <= 0)
		return gbk;
	gbk.resize(len);
	WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, (char*)gbk.data(), len, 0, 0);
	#endif
	return gbk;
}
int main()
{
    std::cout << "Hello World测试!\n";
    //1 测试UTF-8转GBK
	string gdb=UTFToGBK(u8"测试UTF-8转GBK");
	cout<<GBKToUTF8(gdb.c_str());
	
	
	
	//2 测试GBK转UTF-8
	//cout << GBKToUTF8("测试GBK转UTF-8") << endl;//这一步是乱码
	//再转会GBK
	//cout << UTFToGBK(GBKToUTF8("测试GBK转UTF-8").c_str()) << endl;
}
