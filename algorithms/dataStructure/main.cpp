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
#include "matrixGraph.hpp"
#include "ListGraph.hpp"
#include "BinaryTree.hpp"
#include "CircularBuffer.hpp"

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

void printGtaph(const int& val)
{
	cout << val << " ";
}
void matrixGraphTest()
{

	DMatrixGraph<int> test(6);

	test.insertArc(0, 4, 6);
	test.insertArc(1, 0, 9);
	test.insertArc(1, 2, 3);
	test.insertArc(2, 0, 2);
	test.insertArc(2, 3, 5);
	test.insertArc(3, 4, 1);



	DMatrixGraph<int> test2(test);

	test2.foreach(printGtaph);

}

void ListGraphTest()
{
	DListGraph<int > test(6);
	//test.insertSymArc(0, 4, 0, 6);
	//test.insertSymArc(1, 0, 1, 9);
	//test.insertSymArc(1, 2, 1, 3);
	test.insertArc(1, 2, 2, 2);
	test.insertArc(1, 3, 2, 5);
	test.insertArc(1, 4, 2, 5);
	test.insertArc(2, 5, 2, 5);
	test.insertArc(1, 5, 2, 5);

	//test.foreach(printGtaph);

	//test.DFS(1, printGtaph);
	test.BFS(1, printGtaph);
}

inline bool compare(const int& a, const int& b)
{
	return a > b;
}

inline void print(const int& val)
{
	cout << val << endl;
}

int comp(const int& a, const int& b)
{
	if (a == b)
	{
		return 0;
	}
	return a > b ? 1 : -1;
}

void BinaryTreeTest()
{
	DBinaryTree<int> test;

	test.insertBST(5, compare);
	test.insertBST(2, compare);
	test.insertBST(8, compare);
	test.insertBST(888, compare);
	test.insertBST(16, compare);
	test.insertBST(75, compare);
	test.insertBST(111, compare);
	test.insertBST(0, compare);

	test.inOrderForeach(print);

	test.searchBST(111, comp);
}

class foo
{
public:

	~foo()
	{
		printf("~foo!\n");
	}
};


void CircularBufferTest()
{
	CircularBuffer<foo> buf(10);
	
	for (int i = 0; i < 11; ++i)
	{
		foo f;
		buf.push(f);
	}
}

int main(int argc, char* argv[])
{
	
	//arrayTest();
	//linkListTest();
	//stackTest();
	//linkStackTest();
	//sqQueueTest();
	//linkQueueTest();
	//matrixGraphTest();
	//ListGraphTest();
	//BinaryTreeTest();
	CircularBufferTest();

	system("pause");
}