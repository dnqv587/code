#pragma once
#include <vector>
#include <algorithm>
/*
�۲���ģʽ
*/

class Observable;

class Observer
{
public:
	virtual ~Observer();

	//���麯��������չʾ����
	virtual void update() = 0;
	//��ע��۲�
	void observe(Observable* observalbe);
	

protected:
	Observable* m_Obserberable;
};

class Observable
{
public:
	//֪ͨ���й۲���
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
	//ע��
	void register_(Observer* oberserver)
	{
		m_Oberservers.push_back(oberserver);
	}
	//����
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