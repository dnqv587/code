#pragma once
#include <iostream>
#include <functional>

using namespace std;

//二叉树遍历
/*
性质1: 在二叉树的第i层上至多有2^(i-1)个结点（i>0）
性质2: 深度为k的二叉树至多有2^k - 1个结点（k>0）
性质3: 对于任何一棵二叉树，若度为2的结点数有n2个，则叶子数（n0）必定为n2＋1 （即n0=n2+1）    度为2 数 + 1 = 叶子数
性质4: 具有n个结点的完全二叉树的深度必为log2n+1
	(如 log2 (15)    点击 15  log  /  2  log =)
性质5: 对完全二叉树，若从上至下、从左至右编号，则编号为i 的结点，其左孩子编号必为2i，其右孩子编号必为2i＋1；其双亲的编号必为i/2（i＝1 时为根,除外）
*/
/*
满二叉树   深度为k的数 并且节点数量为 2^k - 1
完全二叉树  除了最后一层，上面是一颗满二叉树，在最后一层上只缺少右边的
*/
/*
先序遍历 ：  先根  再左  再右
中序遍历 ：  先左  再根  再右
后序遍历 ：  先左  再右  再根
*/
template <class T>
class DBinaryTree
{
public:
	DBinaryTree():m_NodeNum(0)
	{
		m_root = new treeNode;
		if (m_root == NULL)
		{
			cerr << "内存申请失败" << endl;
			return;
		}
		m_root->lChild = nullptr;
		m_root->rChild = nullptr;
	}

	DBinaryTree(const DBinaryTree* binaryTree):m_NodeNum(0)
	{
		function<treeNode* (treeNode* root)> recursion = [&](treeNode* root)->treeNode*
		{
			if (root == nullptr)
			{
				return nullptr;
			}
			treeNode* lChild = recursion(root->lChild);
			treeNode* rChild = recursion(root->rChild);

			treeNode* newNode = new treeNode;

			newNode->data = root->data;
			newNode->lChild = lChild;
			newNode->rChild = rChild;

			++m_NodeNum;
			return newNode;
		};

		m_root = recursion(binaryTree);
	}

	~DBinaryTree()
	{
		function<void(treeNode* root)> recursion = [&](treeNode* root)->void
		{
			if (root == nullptr)
			{
				return ;
			}

			treeNode* lChild = root->lChild;
			treeNode* rChild = root->rChild;
			delete root;
			recursion(lChild);
			recursion(rChild);
		};

		recursion(m_root);
	}
	
	//二叉排序数的插入
	//中序遍历结果是一个升序排列
	//极值：最小值--最左子节点；最大值---最右子节点
	void insertBST(const T& val, bool(*compare)(const T& a, const T& b))//a比b大返回true
	{
		if (m_NodeNum == 0)
		{
			m_root->data = val;
			++m_NodeNum;
			return;
		}

		function<void(treeNode* root)> recursion = [&](treeNode* root)
		{
			if (compare(val, root->data))
			{
				if (root->rChild == nullptr)
				{
					treeNode* newNode = new treeNode;
					newNode->data = val;
					newNode->lChild = nullptr;
					newNode->rChild = nullptr;
					root->rChild = newNode;
					++m_NodeNum;
					return;
				}
				recursion(root->rChild);
			}
			else
			{
				if (root->lChild == nullptr)
				{
					treeNode* newNode = new treeNode;
					newNode->data = val;
					newNode->lChild = nullptr;
					newNode->rChild = nullptr;
					root->lChild = newNode;
					++m_NodeNum;
					return;
				}
				recursion(root->lChild);

			}
		};

		recursion(m_root);
	}

	//二叉排序树的查找
	//查找到返回true，否则返回false
	void searchBST(const T& val,int (*compare)(const T& a,const T& b))//compare---相等返回0，小于返回-1，大于返回1
	{

		function<void (treeNode* root)> recursion = [&](treeNode* root)
		{
			if (root == nullptr)
			{
				cout << "值:" << val << ",查找失败" << endl;
				return ;
			}
			if (compare(val, root->data) == 0)
			{
				cout << "值:" << val << ",查找成功" << endl;
				return ;
			}
			if (compare(val, root->data) == 1)
			{
				recursion(root->rChild);
			}
			if (compare(val, root->data) == -1)
			{
				recursion(root->lChild);
			}
		};

		recursion(m_root);
	}


	//先序遍历
	void preOrderForeach(void print(const T& val))
	{
		function<void (treeNode* root)> recursion = [&](treeNode* root)->void
		{
			if (root == nullptr)
			{
				return;
			}
			print(root->data);
			recursion(root->lChild);
			recursion(root->rChild);
		};

		recursion(m_root);
	}

	//中序遍历
	void inOrderForeach(void print(const T& val))
	{
		function<void (treeNode* root)> recursion = [&](treeNode* root)->void
		{
			if (root == nullptr)
			{
				return;
			}
			recursion(root->lChild);
			print(root->data);
			recursion(root->rChild);
		};

		recursion(m_root);
	}

	//后序遍历
	void postOrderForeach(void print(const T& val))
	{
		function<void (treeNode* root)> recursion = [&](treeNode* root)->void
		{
			if (root == nullptr)
			{
				return;
			}
			recursion(root->lChild);
			recursion(root->rChild);
			print(root->data);
		};

		recursion(m_root);
	}

	//叶子数量
	size_t leafNumber()
	{
		size_t size = 0;
		function<void(treeNode* root)> recursion = [&](treeNode* root)->void
		{
			if (root == nullptr)
			{
				return ;
			}
			if (root->lChild == nullptr && root->rChild == nullptr)
			{
				++size;
			}
		};
		recursion(m_root);
		return size;
	}

	//树的高度/深度
	size_t depth()
	{
		function<void (treeNode* root)> recursion = [](treeNode* root)->size_t
		{
			if (root == nullptr)
			{
				return 0;
			}
			//左子树的深度
			size_t lDepth = recursion(root->lChild);
			//右子树的深度
			size_t rDepth = recursion(root->rChild);

			size_t depth = lDepth > rDepth ? lDepth + 1 : rDepth + 1;
			return depth;
		};

		return recursion(m_root);
	}

private:
	struct treeNode
	{
		T data;
		treeNode* lChild;
		treeNode* rChild;
	};

private:
	treeNode* m_root;

	size_t m_NodeNum;
};