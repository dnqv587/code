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

void test3()
{
	SelfCopy t1(new int(9));

	SelfCopy t2;
	t2 = t1;

	std::cout << t1.p << ";" << t2.p << std::endl;
	t2 = t2;
	std::cout << t1.p << ";" << t2.p << std::endl;
	
}

int main(int argc, char* argv[])
{
	//test1();
	//test2();
	test3();
	return 0;
}