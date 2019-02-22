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

	UCH* ReadBuff = new UCH[1024];			

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
			cout << _char_info[ReadBuff[i]]._ch << endl;
		}
	}

	//将char_info的数据进行排序，取权值最小的，建立haffman树
	HaffmanTree<Char_info> ht;
	ht.CreateHaffmanTree(_char_info,_char_info[0]);

	//获取haffman编码
	GetHaffmanCode(ht.GetRoot());
	//fseek(Source_File, 0, SEEK_SET);			//将文件指针偏移到文件的开始位置

	FILE* DestFile = fopen("finshFileCompress.hzp", "wb");		//打开压缩文件
	assert(DestFile);

	WriteHead(DestFile, strPath);				//往目标文件中写入后缀，字符编码等信息

	//写压缩文件
	//由于当前打开额源文件的文件指针已经到达文件末尾，所以需要将文件指针重置到开头
	fseek(Source_File, 0, SEEK_SET);
	char ch = 0;
	int bitcount = 0;
	while (true){
		size_t Read_Size = fread(ReadBuff, 1, 1024, Source_File);
		if (0 == Read_Size){
			break;
			//读取文件完毕，结束
		}
		
		//以比特位的方式将字符编码写入到文件中
		for (size_t i = 0; i < Read_Size; ++i){
			string& char_Code = _char_info[ReadBuff[i]]._str_code;
			for (size_t j = 0; j < char_Code.size(); ++j){
				ch <<= 1;
				if ('1' == char_Code[j]){
					ch |= 1;
				}
				bitcount++;
				if (8 == bitcount){
					fputc(ch, DestFile);
					//一个字节一个字节写入文件
					bitcount = 0;
				}
			}
		}
	}
	//写完之后还有比特位剩余，则将剩余的比特位写入文件
	if (bitcount > 0 && bitcount < 8){
		ch <<= (8 - bitcount);				//等于号啊啊啊啊啊
		fputc(ch, DestFile);
	}
	
	
	delete[] ReadBuff;
	fclose(DestFile);			//关闭压缩文件
	fclose(Source_File);		//关闭源文件

	
}



void FileCompressHaffMan::GetHaffmanCode(HaffmanTreeNode<Char_info>* pRoot)	//获取haffman编码
{
	if (nullptr == pRoot){
		return;//没写这个判断条件就会造成死循环
	}
	GetHaffmanCode(pRoot->_pLeft);
	GetHaffmanCode(pRoot->_pRight);

	if ((pRoot->_pLeft == nullptr)&&(pRoot->_pRight == nullptr)){
		HaffmanTreeNode<Char_info>* pCur = pRoot;
		HaffmanTreeNode<Char_info>* pParent = pCur->_pParent;

		string& strCode = _char_info[pCur->_weight._ch]._str_code;
		while (nullptr!=pParent){
			if (pCur == pParent->_pLeft){
				strCode += '0';
			}
			else
			{
				strCode += '1';
			}
			//pParent要往下走，不然会形成死循环
			pCur = pParent;
			pParent = pCur->_pParent;
		}
		reverse(strCode.begin(),strCode.end());		//翻转字符串，将haffman编码写入
	}
}

void FileCompressHaffMan::WriteHead(FILE* DestFile, const string& strPath){
	string str_suffix = strPath.substr(strPath.rfind('.'));			//截取文件的后缀写入目标文件中
	str_suffix += '\n';

	string str_charinfo;
	char szCount[32];
	size_t linecount = 0;

	for (size_t i = 0; i < 256; ++i){
		if (_char_info[i]._char_count){
			str_charinfo += _char_info[i]._ch;
			str_charinfo += '/';
			_itoa(_char_info[i]._char_count, szCount, 10);
			str_charinfo += szCount;
			str_charinfo += '\n';
			linecount++;
		}
	}
	_itoa(linecount, szCount, 10);
	str_suffix += szCount;
	str_suffix += '\n';

	str_suffix += str_charinfo;
	fwrite(str_suffix.c_str(), 1, str_suffix.size(), DestFile);

}

void FileCompressHaffMan::GetLine(FILE* fIn, string& str)//读取一行数据
{
	//feof用来检测文件指针是否到达文件末尾
	while (!feof(fIn)){
		UCH ch = fgetc(fIn);
		if (ch == '\n'){
			return;
		}
		str += ch;
	}
}

void FileCompressHaffMan::UNCompressFile(const string& strPath){
	string& str_suffix = strPath.substr(strPath.rfind('.'));
	if (".hzp" != str_suffix){
		cout << "文件格式不匹配" << endl;
		return;
	}
	FILE* fIn = fopen(strPath.c_str(), "rb");
	if (nullptr == fIn){
		return;
		//打开文件失败
	}
	
	string strfix = "";		//获取文件的后缀
	GetLine(fIn, strfix);

	string strChar = "";
	GetLine(fIn, strChar);
	size_t linecount = atoi(strChar.c_str());
	for (size_t i = 0; i < linecount; ++i){
		strChar = "";
		GetLine(fIn, strChar);
		if (strChar.empty()){
			strChar += '\n';
			GetLine(fIn, strChar);
		}
		_char_info[(UCH)strChar[0]]._char_count = atoi(strChar.c_str() + 2);
		//加2的原因是要把字符以及分隔符跳过去
	}

	//创建haffman树
	HaffmanTree<Char_info> ht;
	ht.CreateHaffmanTree(_char_info, _char_info[0]);
	
	string UNCompress = "finshFileCompress";
	UNCompress += strfix;		//解压后文件的后缀

	FILE* fOut = fopen(UNCompress.c_str(), "wb");
	assert(fOut);

	char* pReadBuff = new char[1024];
	

	HaffmanTreeNode<Char_info>* pCur = ht.GetRoot();

	char pos = 7;
	long long filesize = pCur->_weight._char_count;

	while (true){
		size_t ReadSize = fread(pReadBuff, 1, 1024, fIn);
		if (ReadSize == 0){
			break;//文件读取完毕
		}

		for (size_t i = 0; i < ReadSize; ++i){
			pos = 7;
			char ch = pReadBuff[i];
			for (size_t j = 0; j < 8; ++j){
				//测试代码
				//cout << pReadBuff[j] << endl;
				//cout << (1 << pos) << endl;
				//cout << (pReadBuff[j] & (1 << pos)) << endl;
				if (ch&(1<<pos)){
					pCur = pCur->_pRight;
				}
				else{
					pCur = pCur->_pLeft;
				}

				if (nullptr == pCur->_pLeft&&nullptr == pCur->_pRight){
					fputc(pCur->_weight._ch, fOut);
					pCur = ht.GetRoot();
					filesize--;
					if (0 == filesize){
						break;//解压缩完成
					}
				}
				pos--;
			}
		}
	}
	delete[] pReadBuff;
	fclose(fIn);
	fclose(fOut);
}