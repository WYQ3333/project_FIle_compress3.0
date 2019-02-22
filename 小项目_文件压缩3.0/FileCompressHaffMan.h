#pragma once 

#include<iostream>
#include<vector>
#include<string>
#include<assert.h>
#include"HaffmanTree.hpp"
using namespace std;

typedef unsigned char UCH;

struct Char_info{

	//���캯��
	Char_info(size_t char_count = 0)
		:_char_count(char_count)
	{}

	UCH _ch;			//��¼��ǰ�ַ�
	long long _char_count;			//��¼��ǰ�ַ����ļ��е�λ��
	string _str_code;				//��¼��ǰ�ַ���haffman����

	Char_info operator+(const Char_info& temp)const{
		return Char_info(temp._char_count + _char_count);
	}

	bool operator>(const Char_info& temp)const{
		return _char_count > temp._char_count;
	}

	bool operator<(const Char_info& temp)const{
		return _char_count < temp._char_count;
	}

	bool operator!=(const Char_info& temp)const{
		return _char_count != temp._char_count;
	}

	bool operator==(const Char_info& temp)const{
		return _char_count == temp._char_count;
	}

	bool operator!()const{
		return !_char_count;
	}

};

class FileCompressHaffMan{
public:
	//���캯��
	FileCompressHaffMan();

	void CompressFile(const string& strPath);			//�ļ�ѹ��

	void GetHaffmanCode(HaffmanTreeNode<Char_info>* pRoot);	//��ȡhaffman����

	void WriteHead(FILE* DestFile, const string& strPath);			//д��׺���������Ϣ

	void UNCompressFile(const string& strPath);			//��ѹ�ļ�

	void GetLine(FILE* fIn,string& str);//��ȡһ������

	vector<Char_info> _char_info;
	
};