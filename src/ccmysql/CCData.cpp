/************************FILE INFOMATION***********************************
**
** Project Project       : C++Mysql8.0���ݿ��ƽ̨���ʵս
** Contact               : 913902738@qq.com
** Author                : ����Ȫ
** CCData  �����ݿ��װ���õ����ݽṹ
***************************************************************************/

#include "CCData.h"
#include<fstream>
#include<iostream>
#include<string.h>
#ifdef _WIN32
#include <windows.h> 
#else
#include <iconv.h>
#endif
using namespace std;
#ifndef _WIN32
//Linux�������ַ���ת��
static size_t Convert(char* from_cha, char* to_cha, char* in, size_t inlen, char* out, size_t outlen)
{
	//ת��������
	iconv_t cd;
	cd = iconv_open(to_cha, from_cha);
	if (cd == 0)
		return -1;
	memset(out, 0, outlen);
	char** pin = &in;
	char** pout = &out;
	//����ת���ֽ���������������תGBKʱ��������ȷ >=0�ͳɹ�
	size_t re = iconv(cd, pin, &inlen, pout, &outlen);
	iconv_close(cd);
	return re;
}
#endif

namespace cc {

	CCData::CCData(const int* d)
	{
		this->type = CC_MYSQL_TYPE_LONG;
		this->data = (const char* )d;
		this->size = sizeof(int);
	}
	CCData::CCData(const char* data)
	{
		this->type = CC_MYSQL_TYPE_STRING;
		if (!data)return;
		this->data = data;
		this->size = strlen(data);
	}
	//��ȡ�ļ���С������д��data��size��С
	bool CCData::LoadFile(const char* filename)
	{
		if (!filename)return false;
		fstream in(filename, ios::in | ios::binary);
		if (!in.is_open())
		{
			cerr << "LoadFile" << filename << "failed!" << endl;
			return false;
		}
		//�ļ���С
		in.seekg(0, ios::end);
		size = in.tellg();
		in.seekg(0, ios::beg);
		if (size <= 0)
		{
			return false;
		}
		data = new char[size];
		int readed = 0;
		while(!in.eof())
		{
			in.read((char*)data + readed, size - readed);
			if (in.gcount() > 0)
				readed += in.gcount();
			else
			{
				break;
			}
		}
		in.close();
		this->type = CC_MYSQL_TYPE_BLOB;
		return true;
	}
	//�ͷ� LoadFile����Ŀռ�
	void CCData::Drop()
	{
		delete []data;
		data = NULL;
	}

	bool CCData::SaveFile(const char* filename)
	{
		if (!data || size <= 0)
		{
			return false;
		}
		fstream out(filename, ios::out|ios::binary);
		if (!out.is_open())
		{
			cout << "SaveFile failed!" << filename << "open failed!" << endl;
			return false;
		}
		out.write(data, size);
		out.close();
		return true;
	}
	std::string CCData::UTF8ToGBK() //����ת�����data
	{
		string re = "";
		//1 UFT8 תΪunicode win utf16
#ifdef _WIN32
//WIN�������ַ���ת��
	//1.1 ͳ��ת�����ֽ���
		int len = MultiByteToWideChar(CP_UTF8,	//ת���ĸ�ʽ
			0,			//Ĭ�ϵ�ת����ʽ
			data,		//������ֽ�
			-1,			//������ַ�����С -1 ��\0
			0,//���
			0//����Ŀռ��С
		);
		if (len <= 0)
			return re;
		wstring udata;
		udata.resize(len);
		MultiByteToWideChar(CP_UTF8, 0, data, -1, (wchar_t*)udata.data(), len);

		//2 unicode תGBK
		len = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, 0, 0,
			0,	//ʧ��Ĭ������ַ�
			0	//�Ƿ�ʹ��Ĭ����� 
		);
		if (len <= 0)
			return re;
		re.resize(len);
		WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, (char*)re.data(), len, 0, 0);
#else
		re.resize(1024);
		int inlen = strlen(data);
		Convert((char*)"utf-8", (char*)"gbk", (char*)data, inlen, (char*)re.data(), re.size());
		int outlen = strlen(re.data());
		re.resize(outlen);
#endif
		return re;
	}
	std::string CCData::GBKToUTF8()
	{
		string re = "";
#ifdef _WIN32
//WIN�������ַ���ת��	
		//GBKתunicode
		//1.1 ͳ��ת�����ֽ���
		int len = MultiByteToWideChar(CP_ACP,	//ת���ĸ�ʽ
			0,			//Ĭ�ϵ�ת����ʽ
			data,		//������ֽ�
			-1,			//������ַ�����С -1 ��\0
			0,//���
			0//����Ŀռ��С
		);
		if (len <= 0)
			return re;
		wstring udata;
		udata.resize(len);
		MultiByteToWideChar(CP_ACP, 0, data, -1, (wchar_t*)udata.data(), len);

		//2 unicode תutf-8
		len = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, 0, 0,
			0,	//ʧ��Ĭ������ַ�
			0	//�Ƿ�ʹ��Ĭ����� 
		);
		if (len <= 0)
			return re;
		re.resize(len);
		WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, (char*)re.data(), len, 0, 0);
#else
	//Linux�������ַ���ת��
		re.resize(1024);
		int inlen = strlen(data);
		Convert((char*)"gbk", (char*)"utf-8", (char*)data, inlen, (char*)re.data(), re.size());
		int outlen = strlen(re.data());
		re.resize(outlen);
#endif
		return re;
	}
}