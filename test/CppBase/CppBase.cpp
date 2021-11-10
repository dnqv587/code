#include<stdio.h>
#include <iostream>
#include "./1/Sales_item.h"

int pub=12;
//P18
int _1main(int argc, char** argv)
{
	Sales_item book;
	std::cin >> book;
	std::cout << book << std::endl;
	return 0;
}

int main(int argc, char** argv)
{
	int loc=1;
	//extern int pub;
	int pub = 10;
	int a = 10;
	int b = -42;
	//std::cout << a + b << std::endl;
	std::cout << ::pub<<","<<loc << std::endl;
	int z , &x = z;
	double c = 1.123;
	x = c;
	int *p = &x;
	std::cout << *p << std::endl;
	getchar();
	return 0;
}