#include <iostream>

using namespace std;

//链队
template<class T>
class DLinkQueue
{
public:
	DLinkQueue():m_size(0)
	{
		m_front = new linkNode;
		if (m_front == NULL)
		{
			cerr << "内存申请失败" << endl;
			return;
		}
		m_rear = m_front;
	}

	~DLinkQueue()
	{
		linkNode* nextNode;
		linkNode* curNode = m_front;
		for(int i=0;i<m_size;++i)
		{
			nextNode = curNode->next;
			delete curNode;
			curNode = nextNode;
		}
	}

	//入队
	void push(const T& val)
	{
		m_rear->next = new linkNode;
		m_rear = m_rear->next;
		m_rear->data = val;
		m_rear->next = nullptr;
		++m_size;
	}

	//出队
	T pop()
	{
		if (m_size == 0)
		{
			cerr << "队空" << endl;
			T t;
			return t;
		}

		T front = m_front->next->data;
		linkNode* nextNode = m_front->next->next;
		delete m_front->next;
		m_front->next = nextNode;
		--m_size;
		return front;

	}

	//队头
	T& front()
	{
		if (m_size == NULL)
		{
			return m_front->data;
		}
		return m_front->next->data;
	}

	//队尾
	T& rear()
	{
		if (m_size == NULL)
		{
			return m_front->data;
		}
		return m_rear->data;
	}

	//是否为空
	bool isEmpty()
	{
		if (m_size == 0)
			return true;
		else
			return false;
	}

	//大小
	size_t size()
	{
		return m_size;
	}

private:

	struct linkNode
	{
		T data;
		linkNode* next;
	};

private:

	linkNode* m_front;
	linkNode* m_rear;

	size_t m_size;
};