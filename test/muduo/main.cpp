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

void func(int a, int b, int c)
{
	std::cout << a << " " << b << " " << c << std::endl;
}

void signalTest()
{
	int a = 1,b = 2, c = 3;
	//Signal<func(a,b,c)> sig;
}

int main(int argc, char* argv[])
{
	//observerTest();
	return 0;
}