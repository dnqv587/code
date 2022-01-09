#pragma once
#include <iostream>

using namespace std;


//����
template<class T>
class DArray
{
public:
	//�вι���
	DArray(size_t capacity) :m_capacity(capacity),m_size(0)
	{
		pAddress = new T[m_capacity];
	}
	//��������
	DArray(const DArray& arr)
	{
		m_capacity = arr.m_capacity;
		m_size = arr.m_size;
		pAddress = new T[m_capacity];
		memcpy(pAddress, arr.pAddress, arr.m_size);
	}

	~DArray()
	{
		if (pAddress)
		{
			delete[] pAddress;
			pAddress = nullptr;
		}
	}

	//����=
	DArray& operator= (const DArray& arr)
	{
		if (pAddress)
		{
			delete[] pAddress;
			pAddress = nullptr;
		}
		m_capacity = arr.m_capacity;
		m_size = arr.m_size;
		pAddress = new T[m_capacity];
		memcpy(pAddress, arr.pAddress, arr.m_size);
	}

	//����[]
	T& operator[] (unsigned int index)
	{
		return pAddress[index];
	}

	//β�巨
	void pushBack(const T& val)
	{
		if (m_size >= m_capacity)
		{
			return ;
		}
		pAddress[m_size] = val;
		++m_size;
	}

	//����

	void foreach(void (*print)(const T&))
	{
		if (print == NULL)
		{
			cerr << "�ص�����Ϊ��" << endl;
			return;
		}
		for (int i = 0; i < m_size;++i)
		{
			print(pAddress[i]);
		}
	}


	//��ȡ��������
	size_t getCapacity()
	{
		return m_capacity;
	}

	//��ȡ�����С
	size_t getSize()
	{
		return m_size;
	}



private:
	T* pAddress;//����ָ��

	size_t m_capacity;//����

	size_t m_size;//��С
};