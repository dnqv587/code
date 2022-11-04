#pragma once
#include "../thread/thread.h"
#include "../base/Callbacks.h"
#include "../thread/mutex.h"
#include "../time/Timestamp.h"
#include "../time/TimerID.h"
#include <atomic>
#include <vector>
#include <memory>

//采用one thread per loop 每个线程只能有一个EventLoop实体

class Channel;
class Poller;
class TimerQueue;
class EventLoop :public noncopyable
{

public:
	using Functor = std::function<void()>;

	EventLoop();
	~EventLoop();

	void loop();
	//更新和维护Poller
	void updateChannel(Channel* channel);

	void quit()
	{
		m_quit = true;
		if (!isInLoopThread())
		{
			wakeup();//退出前执行回调函数
		}
	}

	//断言当前线程是否为IO线程，否则abort
	void assertInLoopThread()
	{
		if (!isInLoopThread())
		{
			this->abortNotInLoopThread();
		}
	}

	//当前线程是否是IO线程
	bool isInLoopThread() const
	{
		return CurrentThread::tid() == this->t_threadId;
	}
	//获取当前线程的EventLoop对象
	EventLoop* getEventLoopOfCurrentThread();

	//定时运行函数--支持跨线程调用
	TimerID runAt(const Timestamp time, const TimerCallback& cb);
	TimerID runAfter(double delay, const TimerCallback& cb);
	TimerID runEvery(double interval, const TimerCallback& cb);

	//在IO线程执行任务回调
	void runInLoop(const Functor& cb);
	//加入到IO线程回调队列
	void queueInLoop(const Functor& cb);
	//唤醒IO线程
	void wakeup();

private:
	void abortNotInLoopThread();
	void handleRead();//读出唤醒的数据
	void doPendingFunctors();//执行回调函数

	using ChannelList = std::vector<Channel*>;


	std::atomic<bool> m_looping;//是否在loop
	std::atomic<bool> m_quit;//是否退出loop
	bool m_callPendingFunctor;//唤醒阻塞线程标志位
	Timestamp m_pollReturnTime;//返回时间
	int m_wakeupFd;//唤醒文件描述符
	std::unique_ptr<Channel> m_wakeupChannel;//m_wakeupFd封装，处理readable事件
	MutexLock m_mutex;
	std::vector<Functor> m_pendingFunctors;//待执行的回调函数容器
	bool m_eventHandling;//是否正在处理event事件
	std::unique_ptr<Poller> m_poller;//poller对象
	ChannelList m_activeChannels;//活跃channel集合
	const pid_t t_threadId;//线程ID
	std::unique_ptr<TimerQueue> m_timerQueue;//定时器队列
};

