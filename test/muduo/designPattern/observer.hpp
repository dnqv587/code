#pragma once
#include <vector>
#include <algorithm>
#include<memory>
#include "base/mutex.h"
/*
观察者模式
*/

class Observable;

class Observer :std::enable_shared_from_this<Observer>
{
public:

	virtual ~Observer();

	//纯虚函数，用于展示数据
	virtual void update() = 0;
	//注册观察
	void observe(Observable* observalbe);
	

protected:
	Observable* m_Obserberable;
};

class Observable
{
public:
	//通知所有观察者
	void notify();
	//注册
	void register_(std::weak_ptr<Observer> oberserver);
	/*
	//注销
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
	std::vector<std::weak_ptr<Observer>> m_Oberservers;
	mutable MutexLock m_mutex;
};


Observer::~Observer()
{
	//m_Obserberable->unregister(this);
}

void Observer::observe(Observable* observalbe)
{
	observalbe->register_(shared_from_this());
	m_Obserberable = observalbe;
}

void Observable::notify()
{
	MutexLockGuard lock(m_mutex);//加锁
	auto iter = m_Oberservers.begin();
	while (iter != m_Oberservers.end())
	{
		std::shared_ptr<Observer> obj(iter->lock());//尝试将weak_ptr提升为shared_ptr
		if (obj)
		{
			obj->update();//此为线程安全的，因为obj存在于栈中
			++iter;
		}
		else
		{
			iter = m_Oberservers.erase(iter);//对象已被销毁，从容器中删除
		}
	}
}

void Observable::register_(std::weak_ptr<Observer> oberserver)
{
	m_Oberservers.push_back(oberserver);
}