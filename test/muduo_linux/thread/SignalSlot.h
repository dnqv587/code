#pragma once
#include <vector>
#include <functional>

template <class Signature>
class Signal;

template <class RET,class ...ARGS>
class Signal <RET(ARGS...)>
{
public:
	typedef std::function<void(ARGS...)> functor;

	void connect(functor&& func)
	{
		m_functors.push_back(std::move<functor>(func));
	}

	void call(ARGS&&... args)
	{
		for (const auto& func : m_functors)
		{
			func(args...);
		}
	}

private:
	std::vector<functor> m_functors;

};
