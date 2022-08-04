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
	Timestamp poll(int timeoutMs, ChannelList* activeChannels);

	/*
	* 队列增加所监听Channel
	*/
	void updateChannel(Channel* channel);

	void assertInLoopThread()
	{
		m_ownerLoop->assertInLoopThread();
	}

private:
	//遍历m_pollfds，找出活动事件的fd，将对应事件的Channel填入activeChannels
	void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;

	typedef std::vector<struct pollfd> PollFdList;
	typedef std::map<int, Channel*> ChannelMap;

	EventLoop* m_ownerLoop;//所属于的loop
	PollFdList m_pollfds;//fd容器
	ChannelMap m_channels;

};