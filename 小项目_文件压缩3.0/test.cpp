#include"FileCompressHaffMan.h"
#include <crtdbg.h>    
//����ں����а��� _CrtDumpMemoryLeaks();  

void TestHaffmanTree(){
	vector<int> array = { 1, 3, 5, 7 };
	HaffmanTree<int> ht;
	ht.CreateHaffmanTree(array,0);
}

void TestFileCompress(){
	FileCompressHaffMan f;
	f.CompressFile("1.txt");
	_CrtDumpMemoryLeaks();//��������ڴ�й©��
}

int main(){
	//TestHaffmanTree();
	TestFileCompress();
	system("pasue");
	return 0;
}