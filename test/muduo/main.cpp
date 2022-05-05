#include <iostream>
#include <string>
#include <Windows.h>
#include "designPattern/observer.hpp"
#include "thread/SignalSlot.h"
#include "thread/CountDownLatch.h"
#include "designPattern/singleton.h"

using namespace std;

class Foo : public Observer
{
public:

	virtual void update() override
	{
		printf("Foo::update() %p\n", this);
	}
	void print(const string& str)
	{
		cout << str << endl;
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
int num = 0;

void* Count(void* arg)
{
	int count = *(int*)arg;
	num++;
	Sleep(1000 * count+5000);
	test.countDown();
	return NULL;
}

void CountDownLatchTest()
{
	int count = 0;
	pthread_t thread[4];
	for (int i = 1; i <= 4; ++i)
	{
		pthread_create(&thread[i-1], NULL, Count, &i);
	}
	
	test.wait();
	std::cout << "num:" << num << std::endl;
}


void singletonTest()
{
	Foo ins = Singleton<Foo>::instance();
	ins.print("hello world");
}


int main(int argc, char* argv[])
{
	//observerTest();
	//CountDownLatchTest();
	singletonTest();
	return 0;
}