#include "Timer.h"

std::atomic<int64_t> Timer::s_numCreated;

Timer::Timer(const TimerCallback& cb, Timestamp when, double interval)
	:m_callback(cb),
	m_expiration(when),
	m_interval(interval),
	m_repeat(interval >0.0),
	m_sequence(++s_numCreated)
{

}

void Timer::run() const
{
	m_callback();
}

void Timer::restart(Timestamp now)
{
	if (m_repeat)
	{
		//m_expiration = now.addTime(m_interval);
		m_expiration = now + m_interval;
	}
	else
	{
		m_expiration = Timestamp();
	}
}

Timestamp Timer::expiration()
{
	return m_expiration;
}

bool Timer::repeat()
{
	return m_repeat;
}

int64_t Timer::sequence()
{
	return m_sequence;
}

int64_t Timer::numCreated()
{
	return s_numCreated;
}

