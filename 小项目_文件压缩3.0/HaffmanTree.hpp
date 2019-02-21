#pragma once
#include<vector>
#include<string>
#include<queue>
using namespace std;

//以模板的形式写haffman树

//haffman树的结点信息
template<class T>
struct HaffmanTreeNode{

	typedef HaffmanTreeNode<T> HFNode;

	T _weight;			//haffman树节点的权值，存放文档中字符出现的次数
	HFNode* pLeft;		//haffman树的左孩子
	HFNode* pRight;		//haffman树的右孩子
	HFNode* pParent;	//haffman树的结点的双亲

	//构造函数，初始化haffman树节点的权值以及指针
	HaffmanTreeNode(const T& weight)
		:_weight(weight)
		, pLeft(nullptr)
		, pRight(nullptr)
		, pParent(nullptr)
	{}
};

//自定义priority_queue的排序方式，建立小堆（默认比较方式是>，即默认情况下建立大堆）
template<class T>
struct compare{
	typedef HaffmanTreeNode<T>* pHFNode;
	bool operator()(const pHFNode& pLeft,const pHFNode& pRight){
		return pLeft->_weight > pRight->_weight;
	}
};

//haffman树
template<class T>
class HaffmanTree{
public:
	typedef HaffmanTreeNode<T> HFNode;
	typedef HFNode* pHFNode;

	//v用来存放Char_info结构体
	void CreateHaffmanTree(const vector<T>& v){
		if (v.empty()){
			return;
			//v中为空，则说明信息不存在
		}

		//通过priority_queue建立小堆，对v中的数据进行排序
		priority_queue<pHFNode, vector<pHFNode>, compare<T>> q;
		//入队
		for (size_t i = 0; i < v.size(); ++i){
			/*if (v[i]._char_count! = 0){
				
			}*/
			q.push(new HFNode(v[i]));
		}

		//如果q中的个数大于1.则说明haffman树环没有创建成功
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
			//将新的子树插入优先级队列
			q.push(pParent);
		}
		_pRoot = q.top();			//根节点赋值给_pRoot
	}

	//获取haffman树的根节点
	pHFNode GetRoot(){
		return _pRoot;
	}


private:
	pHFNode _pRoot;			//haffman树的根节点
};