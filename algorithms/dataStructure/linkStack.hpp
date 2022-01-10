#include <iostream>

using namespace std;

//链栈
template <class T>
class DLinkStack
{
public:

	DLinkStack():m_size(0)
	{
		m_base = new linkNode;
		if (m_head == NULL)
		{
			cerr << "内存申请失败" << endl;
			return;
		}
		m_top = m_base;

	}

	~DLinkStack()
	{
		linkNode* curNode = m_base;
		linkNode* nextNode;
		for (int i = 0; i < m_size; ++i)
		{
			nextNode = curNode->next;
			delete curNode;
			curNode = nextNode;
		}
		m_size = 0;
	}

	void push(const T& val)
	{
		m_top->next = new linkNode;
		m_top->next->data = val;
		m_top = m_top->next;
		++m_size;

	}

	void pop()
	{
		linkNode* preNode = m_base;
		for (int i = 0; i < m_size; ++i)
		{
			preNode = preNode->next;
		}
	}

private:

	struct linkNode
	{
		T data;
		linkNode* next;
	};

private:

	linkNode* m_base;//栈底
	linkNode* m_top;//栈顶

	size_t m_size;
};