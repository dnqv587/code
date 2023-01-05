#pragma once
#include "../event/EventLoop.h"
#include "../time/Timestamp.h"

#include <vector>
#include <map>


/*Poller class是对IO多路复用的封装
* Poller是Eventloop的间接成员，只供其owner EventLoop在IO线程调用
* Poller并不拥有Channel对象
*/
struct pollfd;
class Channel;
class Poller //:noncopyable
{
public:
	typedef std::vector<Channel*> ChannelList;

	Poller(EventLoop* loop);
	~Poller();

	/*Poll IO多路复用
	* 必须在loop线程调用
	*/
	virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;

	/*
	* 维护和更新m_pollfds数组,将fd对应的m_pollfds更新或增加Channel中的事件
	* 将fd挂在到内核监控范围中
	*/
	virtual void updateChannel(Channel* channel) = 0;
	/// <summary>
	/// 去除channel，从channel和fd容器
	/// </summary>
	/// <param name="channel"></param>
	virtual void removeChannel(Channel* channel) = 0;


protected:
	void assertInLoopThread()
	{
		m_ownerLoop->assertInLoopThread();
	}

	typedef std::map<int, Channel*> ChannelMap;
	ChannelMap m_channels;//Channel容器

private:
	EventLoop* m_ownerLoop;//所属于的loop

};