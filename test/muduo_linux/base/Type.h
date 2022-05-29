#pragma once
#include <string.h>
#include <stdint.h>
#include <string.h>

#ifndef NDEBUG
#include <assert.h>
#endif

inline void* memZero(void* p, size_t n)
{
	return memset(p, 0x00, n);
}

/*
* 隐式类型转换
* 用于up_cast(向上类型转换)中的安全版本，代替static_cast和const_cast，编译器会检查类型转换是否安全
* 例如，在菱形继承中，若底层对象需要转换成中间对象，此时底层对象可以转换成多个中间对象，会产生\
* 歧义，所以需要类型转换，告诉编译器需要转换的类型
*/
template<typename To,typename From>
inline To implicit_cast(const From& f)
{
	return f;
}

/*
* 向下类型转换
*当父类转换成子类时，是不安全的，此函数确保To为From的父类，且在bebug模式下使用dynamic来检查是否合法
*/
template<typename To, typename From>
inline To down_cast(From* f)
{
	/*
	* 确保To是From的子类
	* 只在编译器进行类型检查
	*/
	if (false)
	{
		implicit_cast<From*, To>(0);
	}
#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
	assert(f == NULL || dynamic_cast<To>(f) != NULL);
#endif
	return static_cast<To>(f);
}