#pragma once
#include <vector>


class Observer;

//被观察者:data
class Oberservalbe
{
public:
	//通知函数：通知所有观察者
	void notify();

	//注册函数
	void _register(Observer* observer);

private:
	//观察者列表---保存所有的观察者
	std::vector<Observer*> m_observers;
};

//观察者
class Observer
{
public:
	//纯虚函数：展示数据
	virtual void update() = 0;

};

