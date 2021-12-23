#include <iostream>
#include <stdio.h>
#include <array>
#include <string>

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
	int cin_int;
	while (cin >> cin_int)
	{
		if (cin_int == 999)
			break;
		pnode->data = cin_int;
		pnode->next = new NODE;
	}
	
}

int main()
{
	//jz3();
	//jz4();
	//jz5();
	jz6();
	system("pause");
}
