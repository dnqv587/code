#include <iostream>
#include "class.h"


void test1()
{
	Vbase* p = new V1;
	delete p;
}

void test2()
{

	Trace* p = new Buy;

	p->log();
}

int main(int argc, char* argv[])
{
	//test1();
	test2();

	return 0;
}