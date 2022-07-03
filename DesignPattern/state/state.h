#pragma once

class Context;
//状态虚基类--设计整个算法实现和状态切换
class State
{
public:
	State();
	//算法结构实现接口--纯虚函数
	virtual void OperationInterface(Context*) = 0;
	//状态切换接口--纯虚函数
	virtual void OperationChangeState(Context*) = 0;
protected:
	//状态切换接口封装--其具体由Context实现
	bool ChangeState(Context* con, State* st);
private:

};

//算法实现A
class ConcreteStateA : public State
{
public:
	ConcreteStateA();
	virtual ~ConcreteStateA();
	virtual void OperationInterface(Context*);
	virtual void OperationChangeState(Context*);
protected:
private:
};

//算法实现B
class ConcreteStateB : public State
{
public:
	ConcreteStateB();
	virtual ~ConcreteStateB();
	virtual void OperationInterface(Context*);
	virtual void OperationChangeState(Context*);
protected:
private:
};

