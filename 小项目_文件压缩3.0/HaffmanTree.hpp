#pragma once
#include<vector>
#include<string>
#include<queue>
using namespace std;

//��ģ�����ʽдhaffman��

//haffman���Ľ����Ϣ
template<class T>
struct HaffmanTreeNode{

	typedef HaffmanTreeNode<T> HFNode;

	T _weight;			//haffman���ڵ��Ȩֵ������ĵ����ַ����ֵĴ���
	HFNode* pLeft;		//haffman��������
	HFNode* pRight;		//haffman�����Һ���
	HFNode* pParent;	//haffman���Ľ���˫��

	//���캯������ʼ��haffman���ڵ��Ȩֵ�Լ�ָ��
	HaffmanTreeNode(const T& weight)
		:_weight(weight)
		, pLeft(nullptr)
		, pRight(nullptr)
		, pParent(nullptr)
	{}
};

//�Զ���priority_queue������ʽ������С�ѣ�Ĭ�ϱȽϷ�ʽ��>����Ĭ������½�����ѣ�
template<class T>
struct compare{
	typedef HaffmanTreeNode<T>* pHFNode;
	bool operator()(const pHFNode& pLeft,const pHFNode& pRight){
		return pLeft->_weight > pRight->_weight;
	}
};

//haffman��
template<class T>
class HaffmanTree{
public:
	typedef HaffmanTreeNode<T> HFNode;
	typedef HFNode* pHFNode;

	//v�������Char_info�ṹ��
	void CreateHaffmanTree(const vector<T>& v){
		if (v.empty()){
			return;
			//v��Ϊ�գ���˵����Ϣ������
		}

		//ͨ��priority_queue����С�ѣ���v�е����ݽ�������
		priority_queue<pHFNode, vector<pHFNode>, compare<T>> q;
		//���
		for (size_t i = 0; i < v.size(); ++i){
			/*if (v[i]._char_count! = 0){
				
			}*/
			q.push(new HFNode(v[i]));
		}

		//���q�еĸ�������1.��˵��haffman����û�д����ɹ�
		while (q.size()>1){
			pHFNode pLeft = q.top();
			q.pop();
			pHFNode pRight = q.top();
			q.pop();

			pHFNode pParent = new HFNode(pLeft->_weight + pRight->_weight);
			pParent->pLeft = pLeft;
			pParent->pRight = pRight;
			pLeft->pParent = pParent;
			pRight->pParent = pParent;
			//���µ������������ȼ�����
			q.push(pParent);
		}
		_pRoot = q.top();			//���ڵ㸳ֵ��_pRoot
	}

	//��ȡhaffman���ĸ��ڵ�
	pHFNode GetRoot(){
		return _pRoot;
	}


private:
	pHFNode _pRoot;			//haffman���ĸ��ڵ�
};