#include "state.h"
#include "context.h"
#include <iostream>

State::State()
{

}

bool State::ChangeState(Context* context, State* state)
{
	if (!state && !context)
	{
		return false;
	}
	std::cout << "State:ChangeState" << std::endl;
	context->ChangeState(state);
	return true;
}

ConcreteStateA::ConcreteStateA()
{

}

ConcreteStateA::~ConcreteStateA()
{

}

void ConcreteStateA::OperationInterface(Context* con)
{
	std::cout << "ConcreteStateA::OperationInterface" << std::endl;
}

void ConcreteStateA::OperationChangeState(Context* con)
{
	std::cout << "ConcreteStateA::OperationChangeState" << std::endl;
	OperationInterface(con);
	this->ChangeState(con, new ConcreteStateB);
}

ConcreteStateB::ConcreteStateB()
{

}

ConcreteStateB::~ConcreteStateB()
{

}

void ConcreteStateB::OperationInterface(Context*)
{
	std::cout << "ConcreteStateB::OperationInterface" << std::endl;
}

void ConcreteStateB::OperationChangeState(Context* con)
{
	std::cout << "ConcreteStateB::OperationChangeState" << std::endl;
	OperationInterface(con);
	this->ChangeState(con, new ConcreteStateA);
}
