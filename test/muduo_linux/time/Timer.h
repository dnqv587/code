#pragma once
#include <functional>
#include <atomic>
#include "../base/noncopyable.h"
#include "Timestamp.h"

//计时器封装类
class Timer :noncopyable
{
public:
	using TimerCallback = std::function<void()>;

	Timer(const TimerCallback& cb, Timestamp when, double interval);

	void run() const;

	void restart(Timestamp now);

	Timestamp expiration();

	bool repeat();

	int64_t sequence();

	static int64_t numCreated();

private:
	TimerCallback m_callback;//回调函数
	Timestamp m_expiration;//运行时间
	const double m_interval;//间隔时间
	const bool m_repeat;//是否为repeat
	const int64_t m_sequence;//定时顺序
	
	static std::atomic<int64_t> s_numCreated;//原子静态对象，所建立的timer数量

};
