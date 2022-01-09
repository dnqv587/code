#pragma once
#include <iostream>

using namespace std;

//��������
template<class T>
struct linkNode
{
	T data;//������
	linkNode* next;//ָ����---��һ���ڵ�ָ��
};

template<class T>
class DLinkList
{
public:
	//�вι���
	DLinkList():m_size(0)
	{
		m_head = new linkNode<T>;
		if (m_head == NULL)
		{
			cerr << "�ڴ�����ʧ��" << endl;
			return;
		}
		m_head->next = nullptr;
		
	}
	//��������
	DLinkList(const DLinkList& list) :m_size(0)
	{
		m_size = list.m_size;
		m_head = new linkNode<T>;
		m_head = list.m_head;
	}

	~DLinkList()
	{
		linkNode<T>* nextNode;
		linkNode<T>* curNode = m_head;
		for (int i = 0; i < m_size; ++i)
		{
			nextNode = curNode->next;
			delete curNode;
			curNode = nextNode;
		}
	}

	T& operator[] (unsigned int index)
	{
		if (index >= m_size)
		{
			cerr << "�����ڴ˽ڵ�" << endl;
			return m_head->data;
		}

		linkNode<T>* curNode = locatePreNode(index);

		return curNode->next->data;
	}

	//β��
	void pushBack(const T& data)
	{
		
		linkNode<T>* curNode = locatePreNode(m_size);
		linkNode<T>* newNode = new linkNode<T>;

		newNode->data = data;
		newNode->next = nullptr;
		curNode->next = newNode;

		++m_size;
	}

	//����
	void insert(int pos,  T data)
	{
		if (pos<0 || pos>m_size)
		{
			cerr << "����λ�ò��ڷ�Χ" << endl;
			return;
		}
		linkNode<T>* preNode = locatePreNode(pos);

		linkNode<T>* newNode = new linkNode<T>;
		newNode->data = data;
		newNode->next = preNode->next;
		preNode->next = newNode;

		++m_size;
	}

	void remove(int pos)
	{
		if (pos < 0 || pos >= m_size)
		{
			cerr << "ɾ��λ�ò��ڷ�Χ" << endl;
			return;
		}

		linkNode<T>* preNode = locatePreNode(pos);

		linkNode<T>* delNode= preNode->next;
		preNode->next = preNode->next->next;

		delete delNode;
		delNode = nullptr;
		
		--m_size;
	}

	void removeByValue(const T& val, bool(*compare)(const T& d_val, const T& f_val))
	{
		linkNode<T>* preNode = m_head;
		linkNode<T>* curNode = m_head->next;
		for (int i = 0; i < m_size; ++i)
		{
			if (compare(val, curNode->data))
			{
				preNode->next = curNode->next;
				delete curNode;
 				curNode = preNode->next;
				--i;
				--m_size;
				//break;
			}
			else
			{
				preNode = preNode->next;
				curNode = curNode->next;
			}
		}
	}

	void clear()
	{
		linkNode<T>* nextNode;
		linkNode<T>* curNode = m_head->next;
		for (int i = 0; i < m_size; ++i)
		{
			nextNode = curNode->next;
			delete curNode;
			curNode = nextNode;
			
		}
		m_size = 0;
	}


	//����
	void foreach(void(*print)(const T&))
	{
		linkNode<T>* curNode = m_head->next;
		for (int i = 0; i < m_size; ++i)
		{
			print(curNode->data);
			curNode = curNode->next;
		}
	}

private:
	//��λǰ���ڵ�
	linkNode<T>* locatePreNode(int pos)
	{
		linkNode<T>* curNode = m_head;
		for (int i = 0; i < pos; ++i)
		{
			curNode = curNode->next;
		}
		return curNode;
	}


private:

	linkNode<T>* m_head;//ͷ���

	size_t m_size;//������

};