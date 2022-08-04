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

void test7()
{
	Foo t1(2);
	//Foo t2 = t1.instansce();//值传递
	Foo& t2 = t1.instansce();//引用传递
	t2.setVal(3);
	std::cout << t1.getVal() << t2.getVal() << std::endl;
}

void test8()
{
	Algo::Math m(3);
	m.print();
	m = m + m;
	m.print();
	m = m + 2;
	m.print();
	int m2 = m + 4;
	std::cout << m2 << std::endl;
}

void test9()
{
	Foo f(10);
	Widget1 w(2, &f);
	Widget1 w2 = w;
	Foo* f2 = w2.getFoo();
	f2->setVal(8);
	
	std::cout << w.getFoo()->getVal() << w2.getFoo()->getVal() << std::endl;
}

int main(int argc, char* argv[])
{
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	//test8();
	test9();
	
	return 0;
}

