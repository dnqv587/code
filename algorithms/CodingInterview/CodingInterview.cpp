#include <iostream>
#include <stdio.h>
#include <array>
#include <string>
#include <functional>
#include <vector>

using namespace std;

//数组中重复的数字
void jz3()
{
	int text[]{ 2,3,1,0,2,5,3 };
	int sign[10000];
	memset(sign, 0x00, sizeof(sign));

	for (auto iter = begin(text); iter != end(text); ++iter)
	{
		if (sign[*iter])
		{
			cout << *iter << endl;
			break;
		}
		sign[*iter] = 1;
	}
}

bool jz4()
{
	int text[][4]{ {1,2,8,9},
					{2,4,9,12},
					{4,7,10,13},
					{6,8,11,15} };
	int count;
	cin >> count;
	if (count<text[0][0] && count>text[3][3])
	{
		cout << "false" << endl;
		return false;
	}


	//sizeof(array[0][0])为一个元素占用的空间，
	//sizeof(array[0])为一行元素占用的空间，
	//sizeof(array)为整个数组占用的空间，

	//行数 = sizeof(array) / sizeof(array[0]);
	//列数 = sizeof(array[0]) / sizeof(array[0][0]);

	size_t array_len = sizeof(text) / sizeof(text[0]);

	for (int i = 0; i < array_len; ++i)
	{
		if (count == text[i][i])
		{
			cout << "true" << endl;
			return true;
		}
		for (int j = i; j >= 0; --j)
		{
			if ((count == text[j][i]) || (count == text[i][j]))
			{
				cout << "true" << endl;
				return true;
			}
		}
	}
	cout << "false" << endl;
	return false;
}

void jz5()
{
	string text = "we are heppy！";

	for (auto iter = text.begin(); iter != text.end(); ++iter)
	{
		if (*iter == ' ')
		{
			*iter = '0';
			iter = text.insert(iter, '2');
			iter = text.insert(iter, '%');
		}
	}
	cout << text << endl;
}

void jz6()
{
	struct NODE
	{
		int data;

		NODE* next;
	};

	NODE *head = new NODE;
	NODE* pNode = head;
	int cin_int;
	int size = 0;
	while (cin >> cin_int)
	{
		if (cin_int == 999)
			break;
		NODE* newNode = new NODE;
		newNode->data = cin_int;
		newNode->next = nullptr;
		pNode->next = newNode;
		pNode = pNode->next;
		++size;
	}

	int node[100];
	NODE* curNode = head->next;
	for (int i = 0; i < size; ++i)
	{
		node[i] = curNode->data;
		curNode = curNode->next;
	}

	for (int i = size-1; i >= 0; --i)
	{
		cout << node[i] << " " << endl;
	}
	
}

void jz7()
{
	struct NODE
	{
		int data;
		NODE* lChild;
		NODE* rChild;
	};
	
	int pre[8]{ 1,2,4,7,3,5,6,8 };
	int vin[8]{ 4,7,2,1,5,3,8,6 };


	//NODE* root = new NODE;
	//root->lChild = nullptr;
	//root->rChild = nullptr;
	//root->data = pre[0];

	//NODE* curNode = root;

	function<NODE* (int preBegin, int preEnd, int InBegin, int InEnd)> func = [&](int preBegin, int preEnd, int InBegin, int InEnd)->NODE*
	{
		if (preBegin==preEnd||InBegin==InEnd)
		{
			return nullptr;
		}

		int vinPos;

		for (int i = InBegin; i < InEnd; ++i)
		{
			if (pre[preBegin] == vin[i])
			{
				vinPos = i;
				break;
			}
		}

		NODE* newNODE = new NODE;
		newNODE->lChild = nullptr;
		newNODE->rChild = nullptr;
		newNODE->data = pre[preBegin];


		newNODE->lChild = func(preBegin + 1, vinPos , InBegin, vinPos);
		newNODE->rChild = func(vinPos + 1, preEnd , vinPos + 1, InEnd);

		return newNODE;

	};


	NODE* root = func(0,7,0,7);

	function<void (NODE* root)> recursion = [&](NODE* root)->void
	{
		if (root == nullptr)
		{
			return;
		}
		cout << root->data << endl;
		recursion(root->lChild);
		recursion(root->rChild);
	};

	recursion(root);
	
}

void jz8()
{
	//组建二叉树
	struct NODE
	{
		int data;
		NODE* lChild;
		NODE* rChild;
		NODE* next;//父节点
	};

	int tree[] = { 1,2,-1,-1,3,-1,4 };
	int flag = 4;

	NODE* root[sizeof(tree) / sizeof(tree[0])];
	root[0] = new NODE;
	root[0]->lChild = nullptr;
	root[0]->rChild = nullptr;
	root[0]->next = nullptr;
	root[0]->data = tree[0];

	for (int i = 1; i < sizeof(tree) / sizeof(tree[0]); ++i)
	{
		if (tree[i] != -1)
		{
			root[i] = new NODE;
			root[i]->lChild = nullptr;
			root[i]->rChild = nullptr;
			root[i]->data = tree[i];
			root[i]->next = root[(i + 1) / 2 - 1];
		}
		else
		{
			root[i] = nullptr;
		}

	}
	for (int i = 0; i < sizeof(tree) / sizeof(tree[0])-2 ; ++i)//有问题
	{
		if (tree[i] != -1)
		{

			root[i]->lChild = root[2 * (i + 1) - 1];

			root[i]->rChild = root[2 * (i + 1)];

		}
	}

	//方法一：
	/*------暴力解法

	vector<int> vec;
	function<void(NODE* curNode)>func = [&](NODE* curNode)->void  
	{
		if (curNode == nullptr)
		{
			return;
		}
		func(curNode->lChild);
		vec.push_back(curNode->data);
		func(curNode->rChild);

	};

	func(root[0]);

	for (auto iter = vec.begin(); iter != vec.end(); ++iter)
	{
		if (*iter == flag)
		{
			cout << "下一个节点为：" << *++iter << endl;
			break;
		}
	}
	*/


	//方法二：
	/*仔细观察，可以把中序下一结点归为几种类型：
1、有右子树，下一结点是右子树中的最左结点
2、无右子树，且结点是该结点父结点的左子树，则下一结点是该结点的父结点
3、无右子树，且结点是该结点父结点的右子树，则一直沿着父结点追朔，直到找到某个结点是其父结点的左子树，如果存在这样的结点，那么这个结点的父结点就是我们要找的下一结点。
4、以上都不存在，则没有下一个节点*/

	function<void(NODE* curNode)>func = [&](NODE* curNode)->void
	{
		if (curNode == nullptr)
		{
			return;
		}
		func(curNode->lChild);
		if (curNode->data == flag)
		{
			if (curNode->rChild != nullptr)//有右子树
			{
				curNode = curNode->rChild;
				while (curNode->lChild != nullptr)
				{
					curNode = curNode->lChild;
				}
				cout << "下一个节点:" << curNode->data << endl;
				return;
			}
			//无右子树
			while (curNode->next != nullptr)
			{
				if (curNode == curNode->next->lChild)
				{
					cout << "下一个节点:" << curNode->next->data << endl;
					return;
				}
				curNode = curNode->next;
			}
			
			
			cout << "没有下一个节点" << endl;
			return;
		}
		func(curNode->rChild);
	};

	func(root[0]);

}

void jz9()
{
	struct NODE
	{
		int data;
		NODE* next;
	};

	class stack
	{
	public:
		stack():m_top(nullptr),m_size(0)
		{

		}

		~stack()
		{
			while (m_top != nullptr)
			{
				NODE* curNODE = m_top;
				m_top = m_top->next;
				delete curNODE;
			}
		}

		void push(const int& val)
		{
			NODE* newNODE = new NODE;
			newNODE->data = val;
			newNODE->next = m_top;
			m_top = newNODE;
			++m_size;
		}

		int pop()
		{
			if (isEmpty())
			{
				throw length_error("栈为空");
			}
			int val = m_top->data;
			NODE* curNODE = m_top;
			m_top = m_top->next;
			delete curNODE;
			--m_size;
			return val;
		}

		bool isEmpty()
		{
			return !m_size;
		}

	private:

		NODE* m_top;
		unsigned int m_size;
	};
	

	stack stack1;
	stack stack2;

	NODE* Queue = nullptr;
	function<void (const int& val)> insert = [&](const int& val)->void
	{
		while (!stack2.isEmpty())
		{
			stack2.pop();
		}
		stack1.push(val);
	};

	function<int ()> front = [&]()->int
	{


		while (!stack1.isEmpty())
		{
			stack2.push(stack1.pop());
		}

		if (stack2.isEmpty())
		{
			throw length_error("stack2 is empty");
		}
		return stack2.pop();
	};

	insert(1);
	insert(2);
	insert(3);

	cout << front() << endl;
	cout << front() << endl;
	cout << front() << endl;

}

void jz10()
{
	function<int(int val)> func = [&](int val)->int 
	{
		if (val <= 2)
		{
			return 1;
		}
		return func(val - 1) + func(val - 2);

	};

	cout << func(4) << endl;
}

void jz11()
{
	int loacte[] = { 3,4,5,1,2 };
	int min;
	
}

int main()
{
	//jz3();
	//jz4();
	//jz5();
	//jz6();
	//jz7();//**
	//jz8();//*
	//jz9();
	//jz10();
	jz11();


	system("pause");
	return 0;
}
