#pragma once
#include <iostream>

using namespace std;

//顺序栈
template <class T>
class DsqStack
{
public:

	DsqStack(size_t capacity) :m_capacity(capacity), m_size(0)
	{
		m_stack = new T[m_capacity];
		if (m_stack == NULL)
		{
			cerr << "内存申请失败" << endl;
			return;
		}

	}

	~DsqStack()
	{
		delete[] m_stack;
	}

	T& operator[] (unsigned int index)
	{
		return m_stack[index];
	}

	//入栈
	void push(const T data)
	{
		if (m_size >= m_capacity)
		{
			cerr << "容量不足" << endl;
			return;
		}

		m_stack[m_size] = data;
		++m_size;

	}

	//出栈
	void pop()
	{
		if (m_size == 0)
		{
			return;
		}
		//memset(m_stack[m_size - 1], 0, sizeof(T));
		--m_size;
	}
	//返回栈顶元素
	T top()
	{
		if (m_size == 0)
		{
			cerr << "栈为空" << endl; 
			T t;
			return  t;
		}
		
		return m_stack[m_size - 1];
	}

	//返回栈大小
	unsigned int size()
	{
		return m_size;
	}

	//栈是否为空
	bool isEmpty()
	{
		if (m_size == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

private:

	//T* m_base;//栈底
	//T* m_top;//栈顶

	T* m_stack;

	size_t m_size;

	size_t m_capacity;

};