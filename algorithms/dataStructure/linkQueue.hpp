#include <iostream>

using namespace std;

//����
template<class T>
class DLinkQueue
{
public:
	DLinkQueue():m_size(0)
	{
		m_front = new linkNode;
		if (m_front == NULL)
		{
			cerr << "�ڴ�����ʧ��" << endl;
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

	//���
	void push(const T& val)
	{
		m_rear->next = new linkNode;
		m_rear = m_rear->next;
		m_rear->data = val;
		m_rear->next = nullptr;
		++m_size;
	}

	//����
	T pop()
	{
		if (m_size == 0)
		{
			cerr << "�ӿ�" << endl;
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

	//��ͷ
	T& front()
	{
		if (m_size == NULL)
		{
			return m_front->data;
		}
		return m_front->next->data;
	}

	//��β
	T& rear()
	{
		if (m_size == NULL)
		{
			return m_front->data;
		}
		return m_rear->data;
	}

	//�Ƿ�Ϊ��
	bool isEmpty()
	{
		if (m_size == 0)
			return true;
		else
			return false;
	}

	//��С
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