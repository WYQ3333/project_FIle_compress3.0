#pragma once 

#include<iostream>
#include<vector>
#include<string>
#include<assert.h>
#include"HaffmanTree.hpp"
using namespace std;

typedef unsigned char UCH;

struct Char_info{

	//构造函数
	Char_info(size_t char_count = 0)
		:_char_count(char_count)
	{}

	UCH _ch;			//记录当前字符
	long long _char_count;			//记录当前字符在文件中的位置
	string _str_code;				//记录当前字符的haffman编码

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
	//构造函数
	FileCompressHaffMan();

	void CompressFile(const string& strPath);			//文件压缩

	void GetHaffmanCode(HaffmanTreeNode<Char_info>* pRoot);	//获取haffman编码

	void WriteHead(FILE* DestFile, const string& strPath);			//写后缀，编码等信息

	void UNCompressFile(const string& strPath);			//解压文件

	void GetLine(FILE* fIn,string& str);//读取一行数据

	vector<Char_info> _char_info;
	
};