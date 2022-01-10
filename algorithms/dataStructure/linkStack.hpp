#include <iostream>

using namespace std;

//链栈
template <class T>
class DLinkStack
{
public:

	DLinkStack():m_size(0)
	{
		m_stack = nullptr;
	}

	~DLinkStack()
	{
		linkNode* curNode = m_stack;
		linkNode* nextNode;
		for (int i = 0; i < m_size; ++i)
		{
			nextNode = curNode->next;
			delete curNode;
			curNode = nextNode;
		}
		m_size = 0;
	}

	//入栈
	void push(const T& val)
	{
		linkNode* curNode = new linkNode;
		curNode->data = val;
		curNode->next = m_stack;
		m_stack = curNode;

		++m_size;

	}

	//出栈
	void pop()
	{
		if (m_size == NULL)
		{
			return;
		}

		linkNode* preNode = m_stack->next;
		delete m_stack;
		m_stack = preNode;
		--m_size;
	}

	//返回栈顶元素
	T& top()
	{
		if (m_size == NULL)
		{
			cerr << "栈空" << endl;
			T t;
			return t;
		}

		return m_stack->data;
	}

	//栈大小
	size_t size()
	{
		return m_size;
	}

	//栈是否为空
	bool isEmpty()
	{
		if (m_size == NULL)
		{
			return true;
		}
		return false;
	}

private:

	struct linkNode
	{
		T data;
		linkNode* next;
	};

private:

	linkNode* m_stack;

	size_t m_size;
};