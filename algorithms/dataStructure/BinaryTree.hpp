#include <iostream>

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
	DBinaryTree()
	{
		m_root = new treeNode;
		if (m_root == NULL)
		{
			cerr << "内存申请失败" << endl;
			return;
		}
	}

	DBinaryTree(const DBinaryTree* binaryTree)
	{
		auto recursion = [](treeNode* root)->treeNode*
		{
			if (root = nullptr)
			{
				return nullptr;
			}
			treeNode* lChild = recursion(root->lChild);
			treeNode* rChild = recursion(root->rChild);

			treeNode* newNode = new treeNode;

			newNode->data = root->data;
			newNode->lChild = lChild;
			newNode->rChild = rChild;
			return newNode;
		};

		m_root = recursion(binaryTree);
	}

	~DBinaryTree()
	{
		auto recursion = [](treeNode* root)->void
		{
			if (root = nullptr)
			{
				return nullptr;
			}

			treeNode* lChild = root->lChild;
			treeNode* rChild = root->rChild;
			delete root;
			recursion(lChild);
			recursion(rChild);
		};

		recursion(m_root);
	}
	
	//先序遍历
	void preOrderForeach(void print(const T& val))
	{
		auto recursion = [&](treeNode* root)->void {if (root == nullptr) { return; }print(root->data); recursion(root->lChild); recursion(root->rChild); };

		recursion(m_root);
	}

	//中序遍历
	void inOrderForeach(void print(const T& val))
	{
		auto recursion = [&](treeNode* root)->void {if (root == nullptr) { return; } recursion(root->lChild); print(root->data); recursion(root->rChild); };

		recursion(m_root);
	}

	//后序遍历
	void postOrderForeach(void print(const T& val))
	{
		auto recursion = [&](treeNode* root)->void {if (root == nullptr) { return; } recursion(root->lChild);  recursion(root->rChild); print(root->data); };

		recursion(m_root);
	}

	//叶子数量
	size_t leafNumber()
	{
		size_t size = 0;
		auto recursion = [&](treeNode* root)->void
		{
			if (root = nullptr)
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
		auto recursion = [](treeNode* root)->size_t
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
};