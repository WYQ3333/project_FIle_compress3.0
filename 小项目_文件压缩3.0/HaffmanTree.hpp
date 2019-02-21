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
	HFNode* _pLeft;		//haffman��������
	HFNode* _pRight;		//haffman�����Һ���
	HFNode* _pParent;	//haffman���Ľ���˫��

	//���캯������ʼ��haffman���ڵ��Ȩֵ�Լ�ָ��
	HaffmanTreeNode(const T& weight)
		:_weight(weight)
		, _pLeft(nullptr)
		, _pRight(nullptr)
		, _pParent(nullptr)
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

	HaffmanTree()
		:_pRoot(nullptr)
	{}
	~HaffmanTree(){
		_delete(_pRoot);
	}

	//v�������Char_info�ṹ��
	void CreateHaffmanTree(const vector<T>& v,const T& invalid){
		if (v.empty()){
			return;
			//v��Ϊ�գ���˵����Ϣ������
		}

		//ͨ��priority_queue����С�ѣ���v�е����ݽ�������
		priority_queue<pHFNode, vector<pHFNode>, compare<T>> q;
		//���
		for (size_t i = 0; i < v.size(); ++i){
			if (v[i] != invalid){
				q.push(new HFNode(v[i]));
			}
			
		}

		//���q�еĸ�������1.��˵��haffman����û�д����ɹ�
		while (q.size()>1){
			pHFNode pLeft = q.top();
			q.pop();
			pHFNode pRight = q.top();
			q.pop();

			pHFNode pParent = new HFNode(pLeft->_weight + pRight->_weight);
			pParent->_pLeft = pLeft;
			pParent->_pRight = pRight;
			pLeft->_pParent = pParent;
			pRight->_pParent = pParent;
			//���µ������������ȼ�����
			q.push(pParent);
		}
		_pRoot = q.top();			//���ڵ㸳ֵ��_pRoot
	}

	//��ȡhaffman���ĸ��ڵ�
	pHFNode GetRoot(){
		return _pRoot;
	}

	//����haffman��
	void _delete(HaffmanTreeNode<T>* pRoot){
		if (pRoot == nullptr){
			return;
		}
		_delete(pRoot->_pLeft);
		_delete(pRoot->_pRight);

		delete pRoot;
	}

private:
	pHFNode _pRoot;			//haffman���ĸ��ڵ�
};