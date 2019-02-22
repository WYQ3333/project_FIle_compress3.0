#define _CRT_SECURE_NO_WARNINGS
#include"FileCompressHaffMan.h"

FileCompressHaffMan::FileCompressHaffMan(){
	_char_info.resize(256);
	for (size_t i = 0; i < _char_info.size(); ++i){
		_char_info[i]._ch = i;
	}
}


void FileCompressHaffMan::CompressFile(const string& strPath){

	//���ļ�����¼��ǰ�ַ��ĳ�������
	if (strPath.empty()){
		cout << "�ļ�Ŀ¼����" << endl;
		return;
	}

	FILE* Source_File = fopen(strPath.c_str(), "rb");		//�Զ����Ƶ���ʽ���ļ�

	if (nullptr == Source_File){
		cout << "���ļ�ʧ��" << endl;
		return;
	}

	//�����ȡ������,����һ�ζ�ȡ������

	UCH* ReadBuff = new UCH[1024];			

	//�����¼�����飬����ͳ���ĵ���ÿ���ַ����ֵĴ���

	

	while (true){
		size_t ReadSize = fread(ReadBuff, 1, 1024, Source_File);
		if (0 == ReadSize){
			//��ȡ�ļ����
			break;
		}

		//ͳ���ĵ����ַ����ֵĴ���
		for (size_t i = 0; i < ReadSize; ++i){
			_char_info[ReadBuff[i]]._char_count++;
			cout << _char_info[ReadBuff[i]]._ch << endl;
		}
	}

	//��char_info�����ݽ�������ȡȨֵ��С�ģ�����haffman��
	HaffmanTree<Char_info> ht;
	ht.CreateHaffmanTree(_char_info,_char_info[0]);

	//��ȡhaffman����
	GetHaffmanCode(ht.GetRoot());
	//fseek(Source_File, 0, SEEK_SET);			//���ļ�ָ��ƫ�Ƶ��ļ��Ŀ�ʼλ��

	FILE* DestFile = fopen("finshFileCompress.hzp", "wb");		//��ѹ���ļ�
	assert(DestFile);

	WriteHead(DestFile, strPath);				//��Ŀ���ļ���д���׺���ַ��������Ϣ

	//дѹ���ļ�
	//���ڵ�ǰ�򿪶�Դ�ļ����ļ�ָ���Ѿ������ļ�ĩβ��������Ҫ���ļ�ָ�����õ���ͷ
	fseek(Source_File, 0, SEEK_SET);
	char ch = 0;
	int bitcount = 0;
	while (true){
		size_t Read_Size = fread(ReadBuff, 1, 1024, Source_File);
		if (0 == Read_Size){
			break;
			//��ȡ�ļ���ϣ�����
		}
		
		//�Ա���λ�ķ�ʽ���ַ�����д�뵽�ļ���
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
					//һ���ֽ�һ���ֽ�д���ļ�
					bitcount = 0;
				}
			}
		}
	}
	//д��֮���б���λʣ�࣬��ʣ��ı���λд���ļ�
	if (bitcount > 0 && bitcount < 8){
		ch <<= (8 - bitcount);				//���ںŰ���������
		fputc(ch, DestFile);
	}
	
	
	delete[] ReadBuff;
	fclose(DestFile);			//�ر�ѹ���ļ�
	fclose(Source_File);		//�ر�Դ�ļ�

	
}



void FileCompressHaffMan::GetHaffmanCode(HaffmanTreeNode<Char_info>* pRoot)	//��ȡhaffman����
{
	if (nullptr == pRoot){
		return;//ûд����ж������ͻ������ѭ��
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
			//pParentҪ�����ߣ���Ȼ���γ���ѭ��
			pCur = pParent;
			pParent = pCur->_pParent;
		}
		reverse(strCode.begin(),strCode.end());		//��ת�ַ�������haffman����д��
	}
}

void FileCompressHaffMan::WriteHead(FILE* DestFile, const string& strPath){
	string str_suffix = strPath.substr(strPath.rfind('.'));			//��ȡ�ļ��ĺ�׺д��Ŀ���ļ���
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

void FileCompressHaffMan::GetLine(FILE* fIn, string& str)//��ȡһ������
{
	//feof��������ļ�ָ���Ƿ񵽴��ļ�ĩβ
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
		cout << "�ļ���ʽ��ƥ��" << endl;
		return;
	}
	FILE* fIn = fopen(strPath.c_str(), "rb");
	if (nullptr == fIn){
		return;
		//���ļ�ʧ��
	}
	
	string strfix = "";		//��ȡ�ļ��ĺ�׺
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
		//��2��ԭ����Ҫ���ַ��Լ��ָ�������ȥ
	}

	//����haffman��
	HaffmanTree<Char_info> ht;
	ht.CreateHaffmanTree(_char_info, _char_info[0]);
	
	string UNCompress = "finshFileCompress";
	UNCompress += strfix;		//��ѹ���ļ��ĺ�׺

	FILE* fOut = fopen(UNCompress.c_str(), "wb");
	assert(fOut);

	char* pReadBuff = new char[1024];
	

	HaffmanTreeNode<Char_info>* pCur = ht.GetRoot();

	char pos = 7;
	long long filesize = pCur->_weight._char_count;

	while (true){
		size_t ReadSize = fread(pReadBuff, 1, 1024, fIn);
		if (ReadSize == 0){
			break;//�ļ���ȡ���
		}

		for (size_t i = 0; i < ReadSize; ++i){
			pos = 7;
			char ch = pReadBuff[i];
			for (size_t j = 0; j < 8; ++j){
				//���Դ���
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
						break;//��ѹ�����
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