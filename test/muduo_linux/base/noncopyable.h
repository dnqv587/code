#pragma once
/*
声明派生类无法使用拷贝构造和拷贝操作符
*/

class noncopyable
{
protected:
	//子类进行构造/析构需要调用基类的构造/析构函数，将此构造和析构设为protected即保证子类构造和析构能通过编译
	noncopyable() = default;
	~noncopyable() = default;

private:
	//同样的子类进行拷贝构造和拷贝操作符前也必须先调用基类的函数，但当前基类的拷贝被私有删除了，所以子类也无法使用拷贝构造和拷贝操作符
	noncopyable(const noncopyable&) = delete;
	noncopyable& operator=(const noncopyable&) = delete;

};
