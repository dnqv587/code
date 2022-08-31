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
#include <windows.h>

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
	Widget1 w(2, new Foo(10));
	Widget1 w2 = w;
	Foo* f2 = w2.getFoo();
	f2->setVal(8);
	
	std::cout << w.getFoo()->getVal() << w2.getFoo()->getVal() << std::endl;
}

void test10()
{
	Widget1 w1(2, new Foo(6));
	Widget1 w2(3, new Foo(7));
	std::cout << "    w1:" << w1.getFoo() << std::endl;
	std::cout << "    w2:" << w2.getFoo() << std::endl;
	//默认版本的swap会调用拷贝构造或拷贝操作符进行拷贝交换
	//重载后的swap会调用特例化后的swap
	std::swap(w1, w2);
	std::cout << "swapw1:" << w1.getFoo() << std::endl;
	std::cout << "swapw2:" << w2.getFoo() << std::endl;
}

void test11()
{
	Base1* A1 = new Impl1;
	//A1->Base1::print();//调用基类中的print
	A1->print();
}

void test12()
{
	std::vector <std::shared_ptr<Base1>> v;
	v.push_back(std::make_shared<Impl1>());
	v.push_back(std::make_shared<Impl2>());
	for (auto iter = v.begin(); iter != v.end(); ++iter)
	{
		if (Impl1* i = dynamic_cast<Impl1*>(iter->get()))  //dynamic_cast为安全的down_cast，在运行时进行down_cast会安全检查，若非安全转换则返回NULL
		{
			i->print();
		}
		else if (Impl2* i = dynamic_cast<Impl2*>(iter->get()))
		{
			i->print();
		}
		
	}
}

template<typename T>
inline T Max(const T& a, const T& b)
{
	return a > b ? a : b;
}

template<typename T>
void test13()
{
	
	T(*pf)(const T & a, const T & b) = Max;//指向函数的指针
	std::cout << Max(10, 12) << std::endl;//这个调用会被inlined，因为是正常调用
	std::cout << pf(8, 4) << std::endl;//这个调用可能不会被inlined，因为它是通过函数指针达成
}

void test14()
{
	Combi com(new IMPL("hello"));
	com.print();
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
	//test9();
	//test10();
	//test11();
	//test12();
	//test13<int>();
	test14();
	
	return 0;
}

