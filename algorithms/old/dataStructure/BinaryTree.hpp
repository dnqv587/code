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

	//平衡二叉树---所有的左子树的与右子树的高度差值（平衡因子）不能大于1

	/*创建平衡二叉树，我们采用依次插入节点的方式进行。而平衡二叉树上插入节点采用递归的方式进行。递归算法如下：
	1、若该树为一空树，那么插入一个数据元素为e的新节点作为平衡二叉树的根节点，树的高度增加1。
	2、若待插入的数据元素e和平衡二叉树的根节点的关键字相等，那么就不需要进行插入操作。
	3、若待插入的元素e比平衡二叉树的根节点的关键字小，而且在平衡二叉树的左子树中也不存在和e有相同关键字的节点，则将e插入在平衡二叉树的左子树上，并且当插入之后的左子树深度增加1时，分别就下列情况处理之：
	4、平衡二叉树的根节点的平衡因子为-1（右子树的深度大于左子树的深度）：则将根节点的平衡因子更改为0，平衡二叉树的深度不变；
	5、平衡二叉树的根节点的平衡因子为0（左右子树的深度相等）：则将根节点的平衡因子修改为1，平衡二叉树的深度增加1；
	6、平衡二叉树的根节点的平衡因子为1（左子树的深度大于右子树的深度）：若平衡二叉树的左子树根节点的平衡因子为1，则需要进行单向右旋转平衡处理，并且在右旋处理后，将根节点和其右子树根节点的平衡因子更改为0，树的深度不变；
	7、若平衡二叉树的左子树根节点的平衡因子为-1，则需进行先向左，后向右的双向旋转平衡处理，并且在旋转处理之后，修改根节点和其左，右子树根节点的平衡因子，树的深度不变；
	8、若e的关键字大于平衡二叉树的根节点的关键字，而且在平衡二叉树的右子树中不存在和e有相同关键字的节点，则将e插入到平衡二叉树的右子树上，并且当插入之后的右子树深度加1时，分别就不同的情况处理之：
	9、平衡二叉树的根节点的平衡因子是1（左子树的深度大于右子树的深度）：则将根节点的平衡因子修改为0，平衡二叉树的深度不变；
	10、平衡二叉树的根节点的平衡因子是0（左右子树的深度相等）：则将根节点的平衡因子修改为-1，树的深度加1；
	11、平衡二叉树的根节点的平衡因子为-1（右子树的深度大于左子树的深度）：若平衡二叉树的右子树根节点的平衡因子为1，则需要进行两次选择，第一次先向右旋转，再向左旋转处理，并且在旋转处理之后，修改根节点和其左，右子树根节点的平衡因子，树的深度不变；
	12、若平衡二叉树的右子树根节点的平衡因子为1，则需要进行一次向左的旋转处理，并且在左旋之后，更新根节点和其左，右子树根节点的平衡因子，树的深度不变；
	*/
	//构建平衡二叉树
	void createBBT() 
	{

	}


	//红黑树
	/*	红黑树的性质
	1、一般的，红黑树，满足以下性质，即只有满足以下全部性质的树，我们才称之为红黑树：
	2、每个结点要么是红的，要么是黑的。
	3、根结点是黑的。
	4、每个叶结点，即空结点（NULL）是黑的。
	5、如果一个结点是红的，那么它的俩个儿子都是黑的。
	6、从根到叶节点的每条路径,必须包含相同数目的黑色节点。
	*/
	
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

	//二叉排序树的删除
	//直接前驱---左子树最右
	//直接后继---右子树最左
	void deleteBST()
	{

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