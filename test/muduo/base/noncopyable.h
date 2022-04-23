#pragma once
/*
声明派生类无法使用拷贝构造和拷贝操作符
*/

class noncopyalbe
{
protected:
	noncopyalbe() = default;
	~noncopyalbe() = default;

private:
	noncopyalbe(const noncopyalbe&) = delete;
	noncopyalbe& operator=(const noncopyalbe&) = delete;

};
