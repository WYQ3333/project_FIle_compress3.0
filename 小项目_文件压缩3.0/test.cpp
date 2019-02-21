#include"FileCompressHaffMan.h"


void TestHaffmanTree(){
	vector<int> array = { 1, 3, 5, 7 };
	HaffmanTree<int> ht;
	ht.CreateHaffmanTree(array);
}

void TestFileCompress(){
	FileCompressHaffMan f;
	f.CompressFile("1.txt");
}

int main(){
	//TestHaffmanTree();
	TestFileCompress();
	system("pasue");
	return 0;
}