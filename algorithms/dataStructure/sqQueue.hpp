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
			cerr << "内存申请失败" << endl;
		}
	}

	~DsqQueue()
	{
		delete[] m_queue;
	}

	//入队
	void push(const T& val)
	{
		if ((m_rear + 1) % m_capacity == m_front)
		{
			cerr << "队满" << endl;
			return;
		}

		m_queue[m_rear] = val;

		m_rear = (m_rear + 1) % m_capacity;
	}
	 
	//出队
	T pop()
	{
		if (isEmpty())
		{
			cerr << "队空" << endl;
			T t;
			return t;
		}
		T top = m_queue[m_front];
		
		m_front = (m_front + 1) % m_capacity;
		return top;
	}

	//返回队头
	T& front()
	{
		return m_queue[m_front];
	}

	//返回队尾
	T& rear()
	{
		return m_queue[m_rear];
	}

	//队是否为空
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

	size_t m_front;//头指针

	size_t m_rear;//尾指针

	size_t m_capacity;

};