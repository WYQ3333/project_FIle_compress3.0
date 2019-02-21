#define _CRT_SECURE_NO_WARNINGS
#include"FileCompressHaffMan.h"

FileCompressHaffMan::FileCompressHaffMan(){
	_char_info.resize(256);
	for (size_t i = 0; i < _char_info.size(); ++i){
		_char_info[i]._ch = i;
	}
}


void FileCompressHaffMan::CompressFile(const string& strPath){

	//打开文件，记录当前字符的出现字数
	if (strPath.empty()){
		cout << "文件目录错误" << endl;
		return;
	}

	FILE* Source_File = fopen(strPath.c_str(), "rb");		//以二进制的形式打开文件

	if (nullptr == Source_File){
		cout << "打开文件失败" << endl;
		return;
	}

	//定义读取缓冲区,接收一次读取的数据

	UCH* ReadBuff = new UCH(1024);			

	//定义记录的数组，用来统计文档中每个字符出现的次数

	

	while (true){
		size_t ReadSize = fread(ReadBuff, 1, 1024, Source_File);
		if (0 == ReadSize){
			//读取文件完成
			break;
		}

		//统计文档中字符出现的次数
		for (size_t i = 0; i < ReadSize; ++i){
			_char_info[ReadBuff[i]]._char_count++;
		}
	}

	//将char_info的数据进行排序，取权值最小的，建立haffman树
	HaffmanTree<Char_info> ht;
	ht.CreateHaffmanTree(_char_info);

	//获取haffman编码
	fseek(Source_File, 0, SEEK_SET);			//将文件指针偏移到文件的开始位置

	
	//获取haffman编码
	GetHaffmanCode(ht.GetRoot());
}

void FileCompressHaffMan::GetHaffmanCode(HaffmanTreeNode<Char_info>* pRoot)	//获取haffman编码
{
	GetHaffmanCode(pRoot->pLeft);
	GetHaffmanCode(pRoot->pRight);
	if (pRoot->pLeft == nullptr&&pRoot->pRight == nullptr){
		HaffmanTreeNode<Char_info>* pCur = pRoot;
		HaffmanTreeNode<Char_info>* pParent = pCur->pParent;

		string& strCode = _char_info[pCur->_weight._ch]._str_code;
		while (pParent){
			if (pCur = pParent->pLeft)
				strCode += '0';
			else
				strCode += '1';
			//pParent要往下走，不然会形成死循环
			pCur = pParent;
			pParent = pCur->pParent;
		}
		reverse(strCode.begin(),strCode.end());		//翻转字符串，将haffman编码写入
	}
}