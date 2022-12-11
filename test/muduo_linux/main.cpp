#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <functional>
#include <queue>
#include <stdint.h>
#include <limits>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/timerfd.h>
#include <signal.h>
#include <time.h>
#include "designPattern/observer.hpp"
#include "thread/SignalSlot.h"
#include "thread/CountDownLatch.h"
#include "designPattern/singleton.h"
#include "test/CopyOnWrite.h"
#include "thread/thread.h"
#include "base/BlockingQueue.h"
#include "logger/LogStream.h"
#include "logger/File.h"
#include "logger/LogFile.h"
#include "time/Timestamp.h"
#include "logger/logging.h"
#include "logger/AsynLogging.h"
#include "thread/ThreadPool.h"
#include "designPattern/prototype.h"
#include "event/EventLoop.h"
#include "event/Channel.h"
#include "time/Timer.h"
#include "net/Poller.h"
#include "time/TimerQueue.h"
#include "net/Socket.h"
#include "net/InetAddress.h"
#include "net/Acceptor.h"


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
	sleep(count + 5);
	test.countDown();
	return NULL;
}

void CountDownLatchTest()
{
	int count = 0;
	pthread_t thread[4];
	for (int i = 1; i <= 4; ++i)
	{
		pthread_create(&thread[i - 1], NULL, Count, &i);
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
		Thread thread1(std::bind(copyTest), to_string(i));
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

BlockingQueue<int> que;
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
		std::cout << ::syscall(SYS_gettid) << ":" << que.take(ConstructorType::MOVE) << std::endl;
	}
}

void blokingQueueTest()
{
	Thread thread1(addThread);
	thread1.start();
	Thread thread2(getThread);
	thread2.start();

	sleep(1);
	int n = 0;
	while (1)
	{
		std::cout << ::syscall(SYS_gettid) << "Z:" << que.take(ConstructorType::MOVE) << std::endl;
		if (n == 9)
		{
			std::cout << "break" << n << std::endl;
			break;
		}
		++n;
	}

	std::queue<int> q = que.drain();
	std::cout << "clear:" << que.size() << std::endl;
	while (!q.empty())
	{
		std::cout << "front:" << q.front() << std::endl;
		sleep(1);
		q.pop();
	}
	getchar();

}

void loggerTest()
{
	const char* a = "Hello";
	const char* b = " World!";
	LogStream log;
	const LogStream::BUFFER& buf = log.buffer();
	log << "Hello";
	log << " World!";
	log << -123456789;
	log.reset();
	log << std::numeric_limits<long>::min();
	log.reset();
	log << 12345678.123456789;
	log.reset();
	int h = 0x123456;
	log << (void*)h;
	log.reset();
	log << format("%4.2f", 1.2) << format("% 4d", 43);
	log.reset();
	log << 12345678901237 << 0.123456789;
	std::cout << buf.toString() << std::endl;
}

void FileTest()
{
	AppendFile file("log.log");
	file.append("test1\n", sizeof("test1\n"));
	file.append("test2\n", sizeof("test2\n"));
	file.append("test3\n", sizeof("test3\n"));
	file.append("end", sizeof("end"));
	file.append("1\n", sizeof("1\n"));
	//file.flush();
	std::cout << file.writtenBytes() << std::endl;
}


static int Fcount = 0;

MutexLock g_lock;
void filethread(LogFile* log)
{

	while (1)
	{
		MutexLockGuard lock(g_lock);
		if (Fcount >= 4096)
			break;
		log->append("filethread1:" + std::to_string(++Fcount) + '\n');
		log->append("filethread2:" + std::to_string(++Fcount) + '\n');
		usleep(200);

	}
}

void LogFileTest()
{
	static LogFile log("main", 1024, true, 3, 64);
	Thread thread(std::bind(filethread, &log));
	thread.start();

	while (1)
	{
		MutexLockGuard lock(g_lock);
		if (Fcount >= 4096)
			break;
		log.append("LogFileTest1:" + std::to_string(++Fcount) + '\n');
		log.append("LogFileTest2:" + std::to_string(++Fcount) + '\n');
		usleep(200);
	}
}

void TimestampTest()
{
	std::cout << Timestamp::now().formatString(true, true) << std::endl;
}

LogFile logFile("SyncLog", 655350000, true, 3, 1024);
void outputFunc(const char* msg, int len)
{
	logFile.append(msg, len);
}

void flushFunc()
{
	logFile.flush();
}

void SyncLogTest()
{
	Logger::setOutput(outputFunc);
	Logger::setFlush(flushFunc);
	std::string line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
	for (int i = 0; i < 4096000; ++i)
	{
		LOG_INFO << line << "---" << i;
		//usleep(1000);
	}


}

AsynLogging ASYNlog("AsynLog", 655350000);
void ASYNoutputFunc(const char* msg, int len)
{
	ASYNlog.append(msg, len);
}

void ASYNflushFunc()
{
	//ASYNlog.flush();
}

void AsynLogTest()
{

	Logger::setOutput(ASYNoutputFunc);
	Logger::setFlush(ASYNflushFunc);
	std::string line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
	ASYNlog.start();
	for (int i = 0; i < 4096000; ++i)
	{
		LOG_INFO << line << "---" << i;
		//usleep(1000);
	}
	ASYNlog.stop();
}

MutexLock mutex;
void addThread1(int n)
{
	{
		MutexLockGuard lock(mutex);
		std::cout << ::syscall(SYS_gettid) << " n:" << n << std::endl;
	}
	sleep(1);
}

void ThreadPoolTest()
{
	ThreadPool threads("threads");
	threads.start(8);
	threads.run(std::bind(&addThread1, 1));
	threads.run(std::bind(&addThread1, 2));
	threads.run(std::bind(&addThread1, 3));
	threads.run(std::bind(&addThread1, 4));

	//getchar();
	threads.stop();
}

void prototypeTest()
{
	Cat cat("coco");
	Dog dog("dodo");

	Animal* cloneCat = cat.Clone();
	Animal* cloneDog = dog.Clone();
	cloneCat->print();
	cloneDog->print();
	((Cat*)cloneCat)->test();
	((Dog*)cloneDog)->test();

	getchar();
}

void Eventtest(EventLoop* c)
{
	std::cout << c->isInLoopThread() << std::endl;
}

void EventLoopTest()
{
	LOG_INFO << "info";
	EventLoop event;
	EventLoop* c = event.getEventLoopOfCurrentThread();
	c->loop();
	Thread t1(std::bind(Eventtest, &event));
	t1.start();
	t1.join();

}

void* timeout(EventLoop* loop, int fd)
{
	printf("Timeout!\n");
	char buf[64];
	::bzero(buf, sizeof buf);
	::read(fd, buf, sizeof buf);
	//loop->quit();
}

void allEventLoopTest()
{
	EventLoop loop;
	int fd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	Channel channel(&loop, fd);
	channel.setReadCallback(std::bind(&timeout, &loop, fd));
	channel.enableReading();

	struct itimerspec howlong;
	bzero(&howlong, sizeof howlong);
	howlong.it_value.tv_sec = 5;
	howlong.it_interval.tv_sec = 3;
	::timerfd_settime(fd, 0, &howlong, NULL);

	loop.loop();

	::close(fd);
}

void timeCb()
{
	printf("time cb\n");
}
void TimerTest()
{
	Timer t(timeCb, Timestamp::now(), 5);

	t.restart(Timestamp::now());
	t.run();
}

void less_than_comparableTest()
{
	Timestamp t1(65535);
	Timestamp t2(65536);
	std::cout << (t2 > t1) << std::endl;
	std::cout << (t1 < t2) << std::endl;
	std::cout << (t2 >= t1) << std::endl;
	std::cout << (t1 <= t2) << std::endl;
}

void timecb(const char* str)
{
	printf("this is %s callback\n", str);
}
void timerCancel(TimerQueue* que, TimerID id)
{
	que->cancel(id);
}
void PollerTest()
{
	EventLoop loop;
	//Poller poll(&loop);

	//Poller::ChannelList list;
	//list.push_back(&channel);
	//poll.poll(0, &list);

	TimerQueue que(&loop);
	TimerID timer1 = que.addTimer(std::bind(&timecb, "timer1"), Timestamp::now() + 5, 3);
	TimerID timer2 = que.addTimer(std::bind(&timecb, "timer2"), Timestamp::now() + 3, 2);
	TimerID timer3 = que.addTimer(std::bind(&timerCancel, &que, timer2), Timestamp::now() + 10, 0);

	loop.loop();

}

void InetAddressTest()
{
	InetAddress ip(8888, true, false);
	if (Endian::isLittleEndian())
	{
		std::cout << ip.ipString() << ip.port() << std::endl;
	}
}

void newConnection(int sockfd, const InetAddress& peerAddr)
{
	std::cout << "newConnection" << peerAddr.ipString() << peerAddr.port() << std::endl;

	::write(sockfd, "How are you?\n", 13);
	Socket::close(sockfd);
}

void AcceptTest()
{
	InetAddress listenAddr(8888);
	EventLoop loop;

	Acceptor accetror(&loop, listenAddr);
	accetror.setNewConnectionCallback(newConnection);
	accetror.listen();
	loop.loop();

}

int main(int argc, char* argv[])
{

	//observerTest();
	//CountDownLatchTest();
	//singletonTest();
	//copyOnWriteTest();
	//blokingQueueTest();
	//loggerTest();
	//FileTest();
	//LogFileTest();
	//TimestampTest();
	//SyncLogTest();
	//AsynLogTest();
	//ThreadPoolTest();
	//prototypeTest();
	//EventLoopTest();
	///allEventLoopTest();
	//TimerTest();
	//less_than_comparableTest();
	//PollerTest();
	//InetAddressTest();
	AcceptTest();

	getchar();
	return 0;
}