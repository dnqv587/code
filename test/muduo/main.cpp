#include <iostream>
#include <string>
#include "designPattern/observer.hpp"
#include "thread/SignalSlot.h"

using namespace std;

class Foo : public Observer
{
public:

	virtual void update() override
	{
		printf("Foo::update() %p\n", this);
	}
};

void observerTest()
{

	Observable subject;
	{
		std::shared_ptr<Foo> p(new Foo);
		p->observe(&subject);
		subject.notify();
	}
	subject.notify();

}

void* count(void* arg)
{
	int* count = (int*)arg;

	std::cout  << " num:" << std::endl;
}

void CountDownLatchTest()
{
	
}

int main(int argc, char* argv[])
{
	//observerTest();
	return 0;
}