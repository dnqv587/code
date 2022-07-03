#include "Template.h"
#include <iostream>

AbstractClass::~AbstractClass()
{

}

void AbstractClass::TemplateMethod()
{
	//整个算法实现框架
	PrimitiveOperation();
}

ConcreteClass1::ConcreteClass1()
{

}

ConcreteClass1::~ConcreteClass1()
{

}

void ConcreteClass1::PrimitiveOperation()
{
	std::cout << "ConcreteClass1 PrimitiveOperation()" << std::endl;
}

ConcreteClass2::ConcreteClass2()
{

}

ConcreteClass2::~ConcreteClass2()
{

}

void ConcreteClass2::PrimitiveOperation()
{
	std::cout << "ConcreteClass2 PrimitiveOperation()" << std::endl;
}
