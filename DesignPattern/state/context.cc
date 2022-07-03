#include "state.h"
#include "context.h"
#include <iostream>

Context::Context()
	:m_state(nullptr)
{

}

Context::Context(State* state)
	:m_state(state)
{

}

Context::~Context()
{
	delete m_state;
}

void Context::OperationInterface()
{
	m_state->OperationInterface(this);
}

void Context::OperationChangeState()
{
	m_state->OperationChangeState(this);
}

bool Context::ChangeState(State* state)
{
	if (!state)
	{
		return false;
	}
	std::cout << "Context:ChangeState" << std::endl;
	if (m_state)
	{
		delete m_state;
		m_state = nullptr;
	}
	this->m_state = state;
	return true;
}
