#pragma once
#include <iostream>

using namespace std;

//˳��ջ
template <class T>
class DsqStack
{
public:

	DsqStack(size_t capacity) :m_capacity(capacity), m_size(0)
	{
		m_stack = new T[m_capacity];
		if (m_stack == NULL)
		{
			cerr << "�ڴ�����ʧ��" << endl;
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

	//��ջ
	void push(const T data)
	{
		if (m_size >= m_capacity)
		{
			cerr << "��������" << endl;
			return;
		}

		m_stack[m_size] = data;
		++m_size;

	}

	//��ջ
	void pop()
	{
		if (m_size == 0)
		{
			return;
		}
		//memset(m_stack[m_size - 1], NULL, sizeof(m_stack[0]));
		--m_size;
	}
	//����ջ��Ԫ��
	T top()
	{
		if (m_size != 0)
		{
			return m_stack[m_size - 1];
		}
		
		cerr << "ջΪ��" << endl;
	}

	//����ջ��С
	unsigned int size()
	{
		return m_size;
	}

	//ջ�Ƿ�Ϊ��
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

	//T* m_base;//ջ��
	//T* m_top;//ջ��

	T* m_stack;

	size_t m_size;

	size_t m_capacity;

};