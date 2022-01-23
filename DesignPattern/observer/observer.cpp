#include "observer.h"

void Oberservalbe::notify()
{
	for (auto observers : m_observers)
	{
		//通知观察者
		observers->update();
	}
}

void Oberservalbe::_register(Observer* observer)
{
	m_observers.push_back(observer);
}

