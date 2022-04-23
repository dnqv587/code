#pragma once
#include <vector>
#include <algorithm>
/*
观察者模式
*/

class Observable;

class Observer
{
public:
	virtual ~Observer();

	//纯虚函数，用于展示数据
	virtual void update() = 0;
	//被注册观察
	void observe(Observable* observalbe);
	

protected:
	Observable* m_Obserberable;
};

class Observable
{
public:
	//通知所有观察者
	void notify()
	{
		for (auto iter : m_Oberservers)
		{
			if (iter)
			{
				iter->update();
			}
		}
	}
	//注册
	void register_(Observer* oberserver)
	{
		m_Oberservers.push_back(oberserver);
	}
	//销毁
	void unregister(Observer* observer)
	{
		auto iter = std::find(m_Oberservers.begin(), m_Oberservers.end(), observer);
		if (iter != m_Oberservers.end())
		{
			std::swap(*iter, m_Oberservers.back());
			m_Oberservers.pop_back();
		}
	}

private:
	std::vector<Observer*> m_Oberservers;
};


Observer::~Observer()
{
	m_Obserberable->unregister(this);
}

void Observer::observe(Observable* observalbe)
{
	observalbe->register_(this);
	m_Obserberable = observalbe;
}