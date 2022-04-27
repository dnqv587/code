#pragma once
/*
声明派生类无法使用拷贝构造和拷贝操作符
*/

class noncopyable
{
protected:
	noncopyable() = default;
	~noncopyable() = default;

private:
	noncopyable(const noncopyable&) = delete;
	noncopyable& operator=(const noncopyable&) = delete;

};
