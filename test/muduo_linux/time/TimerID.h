#pragma once
#include "../base/copyable.h"
#include <stdint.h>

//Timer类封装
class Timer;
class TimerID :public copyable
{
public:
	friend class TimerQueue;

	TimerID()
		:m_timer(nullptr),
		m_sequence(0)
	{

	}

	TimerID(Timer* timer, int64_t sequece)
		:m_timer(timer),
		 m_sequence(sequece)
	{

	}

private:
	Timer* m_timer;
	int64_t m_sequence;//顺序
};