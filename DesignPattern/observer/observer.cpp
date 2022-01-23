#include "observer.h"

void Oberservalbe::notify()
{
	for (auto observers : m_observers)
	{
		//֪ͨ�۲���
		observers->update();
	}
}

void Oberservalbe::_register(Observer* observer)
{
	m_observers.push_back(observer);
}

