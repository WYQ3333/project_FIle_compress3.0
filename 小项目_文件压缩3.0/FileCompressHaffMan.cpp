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

	UCH* ReadBuff = new UCH(1024);			

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
		}
	}

	//��char_info�����ݽ�������ȡȨֵ��С�ģ�����haffman��
	HaffmanTree<Char_info> ht;
	ht.CreateHaffmanTree(_char_info);

	//��ȡhaffman����
	GetHaffmanCode(ht.GetRoot());
	//fseek(Source_File, 0, SEEK_SET);			//���ļ�ָ��ƫ�Ƶ��ļ��Ŀ�ʼλ��

	FILE* DestFile = fopen("1.hzp", "wb");		//��ѹ���ļ�
	assert(DestFile);

	WriteHead(DestFile, strPath);				//��Ŀ���ļ���д���׺���ַ��������Ϣ

}



void FileCompressHaffMan::GetHaffmanCode(HaffmanTreeNode<Char_info>* pRoot)	//��ȡhaffman����
{
	if (nullptr == pRoot){
		return;//ûд����ж������ͻ������ѭ��
	}
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
			//pParentҪ�����ߣ���Ȼ���γ���ѭ��
			pCur = pParent;
			pParent = pCur->pParent;
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
	str_charinfo += szCount;
	str_charinfo += '\n';

	str_suffix += str_charinfo;
	fwrite(str_suffix.c_str(), 1, str_suffix.size(), DestFile);
}