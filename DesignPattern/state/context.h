#pragma once

class State;
//封装类，将算法实现和状态切换封装在此类
class Context
{
	friend class State;
public:
	Context();
	Context(State* state);
	~Context();

	void OperationInterface();
	void OperationChangeState();
private:
	//状态切换的具体实现
	bool ChangeState(State* st);

	State* m_state;

};