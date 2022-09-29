#pragma once

//重载运算符：>,>=,<=
//前提需要先重载<运算符
template<class T>
class less_than_comparable
{
public:
	friend bool operator>(const T& lhs, const T* rhs)
	{
		return rhs < lhs;
	}

	friend bool operator>=(const T& lhs, const T* rhs)
	{
		return !static_cast<bool>(rhs < lhs);
	}

	friend bool operator<=(const T& lhs, const T* rhs)
	{
		return !static_cast<bool>(lhs < rhs);
	}
};