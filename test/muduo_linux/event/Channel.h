#pragma once
#include <functional>

/*
* Reactor最核心的事件分发机制，将IO multiplexing拿到的IO事件分发给各个文件描述符fd的事件处理函数
* 
* Channel是selectable IO channel，负责注册与响应IO事件。但是它没有file descriptor，它是TcpConnection、
* Acceptor、TcpConnection、TimeQueue的成员，其生命周期由后者控制
*/

class EventLoop;
class Channel
{
public:
	typedef std::function<void* ()> EventCallback;

	Channel(EventLoop* loop, const int fd);

	//核心，有EventLoop::loop()调用，根据m_revents的值分别调用不同用户的回调
	void handleEvent();
	//设置读事件回调
	void setReadCallback(const EventCallback& cb)
	{
		m_readCallback = cb;
	}
	//设置写事件回调
	void setWriteCallback(const EventCallback& cb)
	{
		m_writeCallback = cb;
	}
	//设置错误事件回调
	void setErrorCallback(const EventCallback& cb)
	{
		m_errorCallback = cb;
	}
	//监听读事件
	void enableReading()
	{
		m_events |= kReadEvent;
		update();
	}

	void enableWriting()
	{
		m_events |= kWriteEvent;
		update();
	}

	void disableWriting()
	{
		m_events &= ~kWriteEvent;
		update();
	}
	
	void disableALL()
	{
		m_events = kNoneEvent;
		update();
	}

	void set_revents(int revt)
	{
		m_revents = revt;
	}

	int fd() const
	{
		return m_fd;
	}

	int events() const 
	{
		return m_events;
	}

	bool isNoneEvent() const
	{
		return m_events == kNoneEvent;
	}

private:
	void update();

	const int m_fd;//文件描述符
	EventLoop* m_loop;//所属事件循环
	int m_events;//所监听的事件
	int m_revents;//所发生的事件
	int m_index;

	//监听事件
	static const int kNoneEvent;//空事件
	static const int kReadEvent;//读事件|紧急读事件
	static const int kWriteEvent;//写事件


	EventCallback m_readCallback;//读事件回调
	EventCallback m_writeCallback;//写事件回调
	EventCallback m_errorCallback;//错误事件回调
};