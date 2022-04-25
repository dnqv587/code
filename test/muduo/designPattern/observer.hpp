#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>
#include "base/mutex.h"
/*
�۲���ģʽ
*/

class Observable;

class Observer : public std::enable_shared_from_this<Observer>
{
public:

	virtual ~Observer();

	//���麯��������չʾ����
	virtual void update() = 0;
	//ע��۲�
	void observe(Observable* observalbe);
	

protected:
	Observable* m_Obserberable;
};

class Observable
{
public:
	//֪ͨ���й۲���
	void notify();
	//ע��
	void register_(std::weak_ptr<Observer> oberserver);
	/*
	//ע��
	void unregister(Observer* observer)
	{
		auto iter = std::find(m_Oberservers.begin(), m_Oberservers.end(), observer);
		if (iter != m_Oberservers.end())
		{
			std::swap(*iter, m_Oberservers.back());
			m_Oberservers.pop_back();
		}
	}
	*/
private:
	std::vector<std::weak_ptr<Observer>> m_Observers;
	mutable MutexLock m_mutex;
};


Observer::~Observer()
{
	//m_Obserberable->unregister(this);
}

void Observer::observe(Observable* observalbe)
{
	observalbe->register_(std::weak_ptr<Observer> (shared_from_this()));
	m_Obserberable = observalbe;
}

void Observable::notify()
{
	MutexLockGuard lock(m_mutex);//����
	auto iter = m_Observers.begin();
	while (iter != m_Observers.end())
	{
		std::shared_ptr<Observer> obj(iter->lock());//���Խ�weak_ptr����Ϊshared_ptr---���ص�
		if (obj)
		{
			obj->update();//��Ϊ�̰߳�ȫ�ģ���Ϊobj������ջ��
			++iter;
		}
		else
		{
			std::cout << "observer erase!" << std::endl;
			iter = m_Observers.erase(iter);//�����ѱ����٣���������ɾ��
		}
	}
}

void Observable::register_(std::weak_ptr<Observer> oberserver)
{
	m_Observers.push_back(oberserver);
}