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
* ��ʽ����ת��
* ����up_cast(��������ת��)�еİ�ȫ�汾������static_cast��const_cast����������������ת���Ƿ�ȫ
* ���磬�����μ̳��У����ײ������Ҫת�����м���󣬴�ʱ�ײ�������ת���ɶ���м���󣬻����\
* ���壬������Ҫ����ת�������߱�������Ҫת��������
*/
template<typename To,typename From>
inline To implicit_cast(const From& f)
{
	return f;
}

/*
* ��������ת��
*������ת��������ʱ���ǲ���ȫ�ģ��˺���ȷ��ToΪFrom�ĸ��࣬����bebugģʽ��ʹ��dynamic������Ƿ�Ϸ�
*/
template<typename To, typename From>
inline To down_cast(From* f)
{
	/*
	* ȷ��To��From������
	* ֻ�ڱ������������ͼ��
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