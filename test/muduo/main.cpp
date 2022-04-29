#include <iostream>
#include <string>
#include <Windows.h>
#include "designPattern/observer.hpp"
#include "thread/SignalSlot.h"
#include "thread/CountDownLatch.h"

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


CountDownLatch test(4);

void* Count(void* arg)
{
	int* count = (int*)arg;
	Sleep(1000*(*count));
	std::cout  << " num:" << *count<<std::endl;
	test.countDown();
	return NULL;
}

void CountDownLatchTest()
{
	int count = 4;
	pthread_t thread[4];
	for (int i = 1; i <= count; ++i)
	{
		pthread_create(&thread[i-1], NULL, Count, &i);
	}
	
	test.wait();
	std::cout << "CountDownLatchTest Run" << std::endl;
}

int main(int argc, char* argv[])
{
	//observerTest();
	CountDownLatchTest();
	return 0;
}