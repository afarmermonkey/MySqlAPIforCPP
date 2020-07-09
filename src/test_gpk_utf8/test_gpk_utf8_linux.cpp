
#include <iostream>
#include <string>
#include <string.h>
#ifdef _WIN32
#include <windows.h> 
#else
#include <iconv.h>
#endif
using namespace std;

#ifndef _WIN32
static size_t Convert(char *from_cha,char *to_cha,char *in,size_t inlen,char *out,size_t outlen)
{
	//转换上下文
	iconv_t cd;
	cd = iconv_open(to_cha,from_cha);
	if(cd == 0)
		return -1;
	memset(out,0,outlen);
	char **pin = &in;
	char **pout = &out;
	cout<<"in = "<<in<<endl;
	cout<<"inlen = "<<inlen<<endl;
	cout<<"outlen = "<<outlen<<endl;
	//返回转换字节数的数量，但是转GBK时经常不正确 >=0就成功
	size_t re = iconv(cd,pin,&inlen,pout,&outlen);
	iconv_close(cd);
	cout<<"re = "<<(int)re<<endl;
	return re;
}
#endif


string UTF8ToGBK(const char*data)
{
	string re = "";
	//1 UFT8 转为unicode win utf16
#ifdef _WIN32
	//1.1 统计转换后字节数
	int len = MultiByteToWideChar(CP_UTF8,	//转换的格式
		0,			//默认的转换方式
		data,		//输入的字节
		-1,			//输入的字符串大小 -1 找\0
		0,//输出
		0//输出的空间大小
	);
	if (len <= 0)
		return re;
	wstring udata;
	udata.resize(len);
	MultiByteToWideChar(CP_UTF8, 0, data, -1, (wchar_t*)udata.data(), len);

	//2 unicode 转GBK
	len = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, 0, 0,
		0,	//失败默认替代字符
		0	//是否使用默认替代 
	);
	if (len <= 0)
		return re;
	re.resize(len);
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, (char*)re.data(), len,0,0);
#else
	re.resize(1024);
	int inlen = strlen(data);
	cout<<"inlen = "<<inlen<<endl;
	Convert((char*)"utf-8",(char*)"gbk",(char*)data,inlen,(char*)re.data(),re.size());
	int outlen = strlen(re.data());
	cout<<outlen<<endl;
	re.resize(outlen);
#endif
	return re;
}

string GBKToUTF8(const char*data)
{
	string re = "";
#ifdef _WIN32	
	//GBK转unicode
	//1.1 统计转换后字节数
	int len = MultiByteToWideChar(CP_ACP,	//转换的格式
		0,			//默认的转换方式
		data,		//输入的字节
		-1,			//输入的字符串大小 -1 找\0
		0,//输出
		0//输出的空间大小
	);
	if (len <= 0)
		return re;
	wstring udata;
	udata.resize(len);
	MultiByteToWideChar(CP_ACP, 0, data, -1, (wchar_t*)udata.data(), len);

	//2 unicode 转utf-8
	len = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, 0, 0,
		0,	//失败默认替代字符
		0	//是否使用默认替代 
	);
	if (len <= 0)
		return re;
	re.resize(len);
	WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, (char*)re.data(), len, 0, 0);
#else
	re.resize(1024);
	int inlen = strlen(data);
	Convert((char*)"gbk",(char*)"utf-8",(char*)data,inlen,(char*)re.data(),re.size());
	int outlen = strlen(re.data());
	cout<<outlen<<endl;
	re.resize(outlen);
#endif
	return re;
}
int main()
{
	//确保代码是GBK，ansi
    std::cout << "Hello World 测试!\n"; 
	//1 测试UTF-8转GBK
	string in = u8"测试UTF-8转GBK";
	cout<<"输入 utf="<<in<<endl;
	string gbk = UTF8ToGBK(in.c_str()) ;
	cout<<"  gbk = "<<gbk<<endl;
	cout<<GBKToUTF8(gbk.c_str())<<endl;


	//2 测试GBK到UTF-8的转换
	//string uft8 = GBKToUTF8("测试GBK转UTF-8再转为GBK");
	//cout << "utf8=" << uft8 << endl;
	//cout << UTF8ToGBK(uft8.c_str()) << endl;

}
