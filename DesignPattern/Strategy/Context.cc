#include "Context.h"
#include "Strategy.h"

Context::Context(Strategy* strategy)
	:m_strategy(strategy)
{

}

Context::~Context()
{
	delete m_strategy;
}

void Context::DoAction()
{
	m_strategy->PrimitiveOperation();
}
