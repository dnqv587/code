#include "Strategy.h"
#include <iostream>

Strategy::~Strategy()
{

}

Strategy::Strategy()
{

}

ConcreteStrategy1::ConcreteStrategy1()
{

}

ConcreteStrategy1::~ConcreteStrategy1()
{

}

void ConcreteStrategy1::PrimitiveOperation()
{
	std::cout << "ConcreteClass1 PrimitiveOperation()" << std::endl;
}

ConcreteStrategy2::ConcreteStrategy2()
{

}

ConcreteStrategy2::~ConcreteStrategy2()
{

}

void ConcreteStrategy2::PrimitiveOperation()
{
	std::cout << "ConcreteClass2 PrimitiveOperation()" << std::endl;
}
