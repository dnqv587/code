#include <iostream>
#include <stdio.h>
#include <array>

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

int main()
{
	jz3();

	system("pause");
}
