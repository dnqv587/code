#pragma once

//抽象类，用于设计整个算法框架
//将不变的接口在此实现
//变化的接口在派生类中实现
class AbstractClass
{
public:
	virtual ~AbstractClass();
	//基类方法，定义整个算法框架
	void TemplateMethod();

protected:
	//派生类方法，需要派生类去实现
	virtual void PrimitiveOperation() = 0;

};


class ConcreteClass1 :public AbstractClass
{
public:
	ConcreteClass1();
	~ConcreteClass1();
	//算法实现
	void PrimitiveOperation() override;
};

class ConcreteClass2 :public AbstractClass
{
public:

	ConcreteClass2();
	~ConcreteClass2();
	//算法实现
	void PrimitiveOperation() override;
};
