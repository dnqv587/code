#pragma once
#include <iostream>

using namespace std;

//顺序表
template<class T>
class DsqList
{
public:
	//有参构造
	DsqList(size_t capacity) :m_capacity(capacity), m_size(0)
	{
		pAddress = new T[m_capacity];
		if (pAddress == NULL)
		{
			cerr << "内存申请失败" << endl;
			return;
		}
	}
	//拷贝构造
	DsqList(const DsqList& arr)
	{
		m_capacity = arr.m_capacity;
		m_size = arr.m_size;
		pAddress = new T[m_capacity];
		memcpy(pAddress, arr.pAddress, sizeof(T) * arr.m_size);
	}

	~DsqList()
	{
		if (pAddress)
		{
			delete[] pAddress;
			pAddress = nullptr;
		}
	}

	//重载=
	DsqList& operator= (const DsqList& arr)
	{
		if (pAddress)
		{
			delete[] pAddress;
			pAddress = nullptr;
		}
		m_capacity = arr.m_capacity;
		m_size = arr.m_size;
		pAddress = new T[arr.m_capacity];
		//memcpy(pAddress, arr.pAddress, sizeof(T) * arr.m_size);

		for (int i = 0; i < arr.m_size; ++i)
		{
			pAddress[i] = arr.pAddress[i];
		}

		return *this;
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
			extendCapacity(m_capacity * 2);//扩展一倍
		}
		pAddress[m_size] = val;
		++m_size;
	}

	//插入
	void insert(int pos, const T& data)
	{
		
		if (pos > m_size + 1)
		{
			cerr << "插入位置超过大小" << endl;
			return;
		}
		if (m_size >= m_capacity)
		{
			extendCapacity(m_capacity * 2);//扩展一倍
		}

		if (pos == m_size + 1)
		{
			pushBack(data);
			return;
		}

		for (int i = m_size - 1; i >= pos; --i)
		{
			pAddress[i + 1] = pAddress[i];
		}

		pAddress[pos] = data;

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

	//删除
	void remove(int pos)
	{
		if (pos < 0 || pos >= m_size)
		{
			cerr << "删除位置不在范围" << endl;
			return;
		}
		for (int i = pos; i < m_size; ++i)
		{
			pAddress[i] = pAddress[i + 1];
		}

		--m_size;
	}

	//按值删除
	void removeByValue(const T& val,bool(*compare)(const T& d_val,const T&f_val))
	{
		for (int i = 0; i < m_size; ++i)
		{
			if (compare(val, pAddress[i]))
			{
				remove(i);
				i--;
			}
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

	//扩展容量
	void extendCapacity(size_t newCapacity)
	{

		T* newspace = new T[newCapacity];
		memcpy(newspace, pAddress, sizeof(T) * m_capacity);
		m_capacity = newCapacity;

		if (pAddress)
		{
			delete[] pAddress;
			pAddress = nullptr;
		}

		pAddress = newspace;

	}


private:
	T* pAddress;//数组指针

	size_t m_capacity;//容量

	size_t m_size;//大小
};