#include <iostream>
#include "class.h"

#include <string>
#include <iterator>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <vector>

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

void exp()
{
	try
	{
		throw MyException();
		//throw std::bad_alloc();
	}
	catch (std::exception& e)
	{
		throw e;
	}
	catch (MyException& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void test4()
{
	try
	{
		//throw MyException();
		//throw std::bad_alloc();
		exp();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (MyException& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void test5()
{
	Person p("lili", 12);
	PersonWrapper pw(p);
	
	Person p2 = pw;
	std::cout << p2._name << p2._age << std::endl;
}

void test6()
{
	Date date(2022, 3, 23);
	std::cout << date.format() << std::endl;
	std::cout << date.year() << date.month() << date.day() << std::endl;
}

int main(int argc, char* argv[])
{
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	test6();
	return 0;
}

