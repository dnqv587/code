#pragma once

//Strategy模式
//虚基类，设计整个算法实现
class Strategy
{
public:
	virtual ~Strategy();
	virtual void PrimitiveOperation() = 0;
protected:
	//不能给外部构造
	Strategy();
};

class ConcreteStrategy1 :public Strategy
{
public:
	ConcreteStrategy1();
	~ConcreteStrategy1();
	void PrimitiveOperation();
};

class ConcreteStrategy2 :public Strategy
{
public:
	ConcreteStrategy2();
	~ConcreteStrategy2();
	void PrimitiveOperation();
};
