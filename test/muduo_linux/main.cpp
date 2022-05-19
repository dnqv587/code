#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <functional>
#include <queue>
#include "designPattern/observer.hpp"
#include "thread/SignalSlot.h"
#include "thread/CountDownLatch.h"
#include "designPattern/singleton.h"
#include "test/CopyOnWrite.h"
#include "thread/thread.h"
#include "base/BlockingQueue.h"

using namespace std;

class Foo : public Observer
{
public:
	Foo()
	{
		std::cout << "Foo create" << std::endl;
		str = new char[64];
	}

	~Foo()
	{
		std::cout << "Foo destroy" << std::endl;
		delete[] str;
	}

	virtual void update() override
	{
		printf("Foo::update() %p\n", this);
	}
	void print(const string& str)
	{
		cout << str << endl;
	}
private:
	char* str;
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
	sleep(count+5);
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
	Foo* ins = Singleton<Foo>::instance();
	ins->print("hello world");
	std::cout << "func death" << std::endl;
}

Customer cus;
void copyTest()
{
	//Customer* cus = (Customer*)arg;
	
	sleep(1);
	while (1)
	{
		sleep(1);
		std::cout << cus.quary("B") << std::endl;

		std::cout << CurrentThread::name() << std::endl;
		std::cout << CurrentThread::tid() << std::endl;
		std::cout << CurrentThread::isMainThread() << std::endl;
	}
	
}

void copyOnWriteTest()
{
	
	//pthread_t thread[5];
	for (int i = 0; i < 5; ++i)
	{
		//pthread_create(&thread[i], NULL, copyTest, &cus);
		Thread thread1(std::bind(copyTest),to_string(i));
		thread1.start();
		
	}

	cus.update("A", 1);
	cus.update("B", 2);
	cus.update("C", 3);
	int count = 0;
	sleep(0.5);
	while (1)
	{
		cus.update("B", count);
		++count;
		sleep(1);
		cus.destroy("B");
	}
	
	
}

BlokingQueue<int> que;
void addThread()
{
	for (int i = 0; i < 10; ++i)
	{
		sleep(1);
		que.put(i);
	}
}
void getThread()
{
	while (1)
	{
		std::cout << que.take() << std::endl;
	}
}

void blokingQueueTest()
{
	Thread thread1(addThread);
	thread1.start();
	Thread thread2(getThread);
	//thread2.start();
	
	sleep(1);
	while (1)
	{
		//std::cout << que.take() << std::endl;
		sleep(1);
		std::cout << que.size() << std::endl;
		if (que.size() == 10)
			break;
	}
	
	std::queue<int> q = que.drain();
	std::cout << "clear:" << que.size() << std::endl;
	while (!q.empty())
	{
		std::cout << q.front() << std::endl;
		sleep(1);
		q.pop();
	}
	getchar();

}

int main(int argc, char* argv[])
{
	//observerTest();
	//CountDownLatchTest();
	//singletonTest();
	//copyOnWriteTest();
	blokingQueueTest();
	return 0;
}