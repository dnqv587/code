#pragma once
#include <iostream>

using namespace std;

//˳���
template<class T>
class DsqList
{
public:
	//�вι���
	DsqList(size_t capacity) :m_capacity(capacity), m_size(0)
	{
		pAddress = new T[m_capacity];
		if (pAddress == NULL)
		{
			cerr << "�ڴ�����ʧ��" << endl;
			return;
		}
	}
	//��������
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

	//����=
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
			extendCapacity(m_capacity * 2);//��չһ��
		}
		pAddress[m_size] = val;
		++m_size;
	}

	//����
	void insert(int pos, const T& data)
	{
		
		if (pos > m_size + 1)
		{
			cerr << "����λ�ó�����С" << endl;
			return;
		}
		if (m_size >= m_capacity)
		{
			extendCapacity(m_capacity * 2);//��չһ��
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

	//ɾ��
	void remove(int pos)
	{
		if (pos < 0 || pos >= m_size)
		{
			cerr << "ɾ��λ�ò��ڷ�Χ" << endl;
			return;
		}
		for (int i = pos; i < m_size; ++i)
		{
			pAddress[i] = pAddress[i + 1];
		}

		--m_size;
	}

	//��ֵɾ��
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

	//��չ����
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
	T* pAddress;//����ָ��

	size_t m_capacity;//����

	size_t m_size;//��С
};