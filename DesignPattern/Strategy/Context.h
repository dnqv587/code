#pragma once

class Strategy;
//封装类，通过组合的方式将算法组合在此类当中
class Context
{
public:
	Context(Strategy* strategy);
	~Context();
	//调用被封装的算法
	void DoAction();

private:
	Strategy* m_strategy;
};
