#include"FileCompressHaffMan.h"
#include <crtdbg.h>    
//在入口函数中包含 _CrtDumpMemoryLeaks();  

void TestHaffmanTree(){
	vector<int> array = { 1, 3, 5, 7 };
	HaffmanTree<int> ht;
	ht.CreateHaffmanTree(array,0);
}

void TestFileCompress(){
	FileCompressHaffMan f;
	f.CompressFile("1.txt");
	_CrtDumpMemoryLeaks();//用来检测内存泄漏的
}

int main(){
	//TestHaffmanTree();
	TestFileCompress();
	system("pasue");
	return 0;
}