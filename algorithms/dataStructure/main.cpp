#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "array.hpp"
#include "sqList.hpp"
#include "linkList.hpp"
#include "sqStack.hpp"
#include "linkStack.hpp"
#include "sqQueue.hpp"
#include "linkQueue.hpp"

using namespace std;

class Person
{
public:
	Person() = default;

	Person(string name, int age)
	{
		m_name = name;
		m_age = age;
	}

	string m_name;
	int m_age;

};


void print( const Person& arr)
{
	cout << arr.m_name << arr.m_age << endl;
}

bool compare(const Person& a, const Person& b)
{
	if (a.m_age == b.m_age && a.m_name == b.m_name)
	{
		return true;
	}
	return false;
}

void arrayTest()
{

	DsqList<Person>arr(10);

	DsqList<Person>*arr2 = new DsqList<Person>(10);

	DsqList<Person>arr3(*arr2);

	Person p1("abc", 1);
	Person p2("测试", 2);
	Person p3("haha", 3);


	arr.pushBack(p1);
	arr.pushBack(p2);
	arr.pushBack(p2);
	arr.pushBack(p2);

	arr.insert(0, p3);

	*arr2 = arr;
	arr.removeByValue(p2,compare);
	arr.remove(0);

	arr.foreach(print);


	arr3 = *arr2;
	cout << "-------" << endl;
	arr2->foreach(print);

	cout << "-------" << endl;
	delete arr2;
	arr3.foreach(print);

	
}

void linkListTest()
{
	DLinkList<Person> link1;
	Person p1("abc", 1);
	Person p2("测试", 2);
	Person p3("haha", 3);

	link1.pushBack(p1);
	link1.pushBack(p2);
	link1.pushBack(p3);
	link1.pushBack(p3);
	link1.pushBack(p3);

	link1.insert(0,p3);

	link1.remove(0);

	link1.removeByValue(p3,compare);
	//link1.clear();

	link1.foreach(print);

	cout << "---" << link1[0].m_name << endl;
	
}

void stackTest()
{
	DsqStack<Person>* test = new DsqStack<Person>(10);
	Person p1("abc", 1);
	Person p2("测试", 2);
	Person p3("haha", 3);
	
	test->push(p1);
	test->push(p2);
	test->push(p3);

	for (int i = 0; i < 3; ++i)
	{
		cout << test->top().m_name << test->top().m_age << endl;
		test->pop();
	}

	cout << test->top().m_name << endl;

}

void linkStackTest()
{
	DLinkStack<Person> test;
	Person p1("abc", 1);
	Person p2("测试", 2);
	Person p3("haha", 3);

	test.push(p1);
	test.push(p2);
	test.push(p3);

	for (int i = 0; i < 3; ++i)
	{
		cout << test.top().m_name << test.top().m_age << endl;
		test.pop();
	}

	

}

void sqQueueTest()
{
	DsqQueue<Person> test(4);
	Person p1("abc", 1);
	Person p2("测试", 2);
	Person p3("haha", 3);

	test.push(p1);
	test.push(p2);
	test.push(p3);

	for (int i = 0; i < 3; ++i)
	{
		cout << test.front().m_name << test.front().m_age << endl;
		test.pop();
	}

	test.push(p2);
	cout << test.rear().m_name << test.size() << test.isEmpty() << endl;

}

void linkQueueTest()
{
	DLinkQueue<Person> test;
	Person p1("abc", 1);
	Person p2("测试", 2);
	Person p3("haha", 3);

	test.push(p1);
	test.push(p2);
	test.push(p3);
	test.push(p3);
	test.push(p3);
	test.push(p3);
	test.push(p3);
	test.push(p3);

	for (int i = test.size(); i > 0; --i)
	{
		cout << test.front().m_name << test.front().m_age << endl;
		test.pop();
	}

	test.push(p2);
	cout << test.rear().m_name << test.size() << test.isEmpty() << endl;
}

int main(int argc, char* argv[])
{
	
	//arrayTest();
	//linkListTest();
	//stackTest();
	//linkStackTest();
	//sqQueueTest();
	linkQueueTest();

	system("pause");
}