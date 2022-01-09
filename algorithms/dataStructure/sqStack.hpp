#pragma once
#include <iostream>

using namespace std;

//À≥–Ú’ª
template <class T>
class DsqStack
{
public:

	DsqStack(size_t capacity) :m_capacity(capacity), m_size(0)
	{
		m_stack = new T[m_capacity];
		if (m_stack == NULL)
		{
			cerr << "ƒ⁄¥Ê…Í«Î ß∞‹" << endl;
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

	//»Î’ª
	void push(const T data)
	{
		if (m_size >= m_capacity)
		{
			cerr << "»›¡ø≤ª◊„" << endl;
			return;
		}

		m_stack[m_size] = data;
		++m_size;

	}

	//≥ˆ’ª
	void pop()
	{
		if (m_size == 0)
		{
			return;
		}
		//memset(m_stack[m_size - 1], NULL, sizeof(m_stack[0]));
		--m_size;
	}
	//∑µªÿ’ª∂•‘™Àÿ
	T top()
	{
		if (m_size != 0)
		{
			return m_stack[m_size - 1];
		}
		
		cerr << "’ªŒ™ø’" << endl;
	}

	//∑µªÿ’ª¥Û–°
	unsigned int size()
	{
		return m_size;
	}

	//’ª «∑ÒŒ™ø’
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

	//T* m_base;//’ªµ◊
	//T* m_top;//’ª∂•

	T* m_stack;

	size_t m_size;

	size_t m_capacity;

};