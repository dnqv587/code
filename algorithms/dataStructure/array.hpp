#pragma once
#include <iostream>

using namespace std;


//数组
template<class T>
class DArray
{
public:
	//有参构造
	DArray(size_t capacity) :m_capacity(capacity),m_size(0)
	{
		pAddress = new T[m_capacity];
	}
	//拷贝构造
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

	//重载=
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

	//重载[]
	T& operator[] (unsigned int index)
	{
		return pAddress[index];
	}

	//尾插法
	void pushBack(const T& val)
	{
		if (m_size >= m_capacity)
		{
			return ;
		}
		pAddress[m_size] = val;
		++m_size;
	}

	//遍历

	void foreach(void (*print)(const T&))
	{
		if (print == NULL)
		{
			cerr << "回调函数为空" << endl;
			return;
		}
		for (int i = 0; i < m_size;++i)
		{
			print(pAddress[i]);
		}
	}


	//获取数组容量
	size_t getCapacity()
	{
		return m_capacity;
	}

	//获取数组大小
	size_t getSize()
	{
		return m_size;
	}



private:
	T* pAddress;//数组指针

	size_t m_capacity;//容量

	size_t m_size;//大小
};