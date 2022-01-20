#include <iostream>
#include <stdio.h>
#include <array>
#include <string>
#include <functional>

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
	struct NODE
	{
		int data;
		NODE* lChild;
		NODE* rChild;
		NODE* next;
	};

	int tree[] = { 8,6,10,5,7,9,11 };
	int flag = 8;

	NODE* root[sizeof(tree) / sizeof(tree[0])];
	root[0] = new NODE;
	root[0]->lChild = nullptr;
	root[0]->rChild = nullptr;
	root[0]->next = nullptr;
	root[0]->data = tree[0];

	for (int i = 1; i < sizeof(tree) / sizeof(tree[0]); ++i)
	{
		root[i] = new NODE;
		root[i]->lChild = nullptr;
		root[i]->rChild = nullptr;
		root[i]->data = tree[i];
		root[i]->next = root[(i + 1) / 2 - 1];

	}
	for (int i = 0; i < sizeof(tree) / sizeof(tree[0]) /2; ++i)
	{
		root[i]->lChild = root[(i + 1) * 2 - 1];
		root[i]->rChild = root[(i + 1) * 2];
	}

	NODE* cur = root[0];
	function<void(NODE* curNode)>func = [&](NODE* curNode)->void  
	{
		if (curNode == nullptr)
		{
			return;
		}
		func(curNode->lChild);
		if (curNode->data == flag)
		{
			cout << curNode->rChild->data << endl;
		}

		func(curNode->rChild);

	};

	func(root[0]);

}

int main()
{
	//jz3();
	//jz4();
	//jz5();
	//jz6();
	//jz7();//*
	jz8();


	system("pause");
	return 0;
}
