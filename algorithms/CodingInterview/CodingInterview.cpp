#include <iostream>
#include <stdio.h>
#include <array>

using namespace std;

//数组中重复的数字
void jz3()
{
	int text[]{ 2,3,1,0,2,5,3 };

	unsigned short mask = 0x00;//掩码，判断该数字是否有数字
	unsigned short sign = 0x01;

	for (auto iter = begin(text); iter != end(text); ++iter)
	{
		printf("mask:%X\n", mask);
		if ((sign << *iter) && mask)
		{
			cout << *iter<<endl;
			break;
		}
		mask |= sign << *iter;
	}
}

int main()
{
	jz3();

	system("pause");
}
