#pragma once
#include <iostream>

using namespace std;

//单向链表
//template<class T>

template<class T>
class DLinkList
{
public:
	//有参构造
	DLinkList():m_size(0)
	{
		m_head = new linkNode<T>;
		if (m_head == NULL)
		{
			cerr << "内存申请失败" << endl;
			return;
		}
		m_head->next = nullptr;
		
	}
	//拷贝构造
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
			cerr << "不存在此节点" << endl;
			return m_head->data;
		}

		linkNode<T>* curNode = locatePreNode(index);

		return curNode->next->data;
	}

	//尾插
	void pushBack(const T& data)
	{
		
		linkNode<T>* curNode = locatePreNode(m_size);
		linkNode<T>* newNode = new linkNode<T>;

		newNode->data = data;
		newNode->next = nullptr;
		curNode->next = newNode;

		++m_size;
	}

	//插入
	void insert(int pos,  T data)
	{
		if (pos<0 || pos>m_size)
		{
			cerr << "插入位置不在范围" << endl;
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
			cerr << "删除位置不在范围" << endl;
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


	//遍历
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

	template<class T>
	struct linkNode
	{
		T data;//数据域
		linkNode* next;//指针域---下一个节点指针
	};

	//定位前驱节点
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

	linkNode<T>* m_head;//头结点

	size_t m_size;//链表长度

};