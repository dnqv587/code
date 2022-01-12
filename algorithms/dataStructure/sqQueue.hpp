#include <iostream>

using namespace std;

template <class T>
class DsqQueue
{
public:
	DsqQueue(size_t capcacity) :m_capacity(capcacity), m_front(0),m_rear(0)
	{
		m_queue = new T[m_capacity];
		if (m_queue == NULL)
		{
			cerr << "�ڴ�����ʧ��" << endl;
		}
	}

	~DsqQueue()
	{
		delete[] m_queue;
	}

	//���
	void push(const T& val)
	{
		if ((m_rear + 1) % m_capacity == m_front)
		{
			cerr << "����" << endl;
			return;
		}

		m_queue[m_rear] = val;

		m_rear = (m_rear + 1) % m_capacity;
	}
	 
	//����
	T pop()
	{
		if (isEmpty())
		{
			cerr << "�ӿ�" << endl;
			T t;
			return t;
		}
		T top = m_queue[m_front];
		
		m_front = (m_front + 1) % m_capacity;
		return top;
	}

	//���ض�ͷ
	T& front()
	{
		return m_queue[m_front];
	}

	//���ض�β
	T& rear()
	{
		return m_queue[m_rear];
	}

	//���Ƿ�Ϊ��
	bool isEmpty()
	{
		if (m_front == m_rear)
		{
			return true;
		}
		return false;
	}

	size_t size()
	{
		return (m_rear - m_front + m_capacity) % m_capacity;
	}

private:

	T* m_queue;

	size_t m_front;//ͷָ��

	size_t m_rear;//βָ��

	size_t m_capacity;

};