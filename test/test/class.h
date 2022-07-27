#pragma once
#include <stdio.h>
#include <string>
#include <utility>

//virtual析构测试
class Vbase
{
public:
	Vbase()
	{
		printf("constructor Vbase!\n");
	}
	/*
	* 1、使用virtual析构函数，在使用几类指针指向派生类对象时，如果不声明为virtual，则delete不会调用派生类的析构函数
	* 2、若将基类声明为纯虚析构函数virtual ~Vbase()=0 ，当delete时会从派生类到基类进行调用析构函数，此时基类并没有进行实现，从而产生连接错误
	*/
	virtual ~Vbase()
	{
		printf("destructor Vbase!\n");
	}
	
};


class V1:public Vbase
{
public:
	V1()
	{
		printf("constructor V1!\n");
	}

	~V1() override 
	{
		printf("destructor V1!\n");
	}

};


//9、绝不在构造和析构过程中调用virtual函数

class Trace
{
public:
	Trace()
	{
		log();
	}

	virtual void log() const
	{
		printf("Trace\n");
	}
};

class Buy:public Trace
{
public:
	void log() const override
	{
		printf("Buy\n");
	}
};

class Sale :public Trace
{
public:
	void log() const override
	{
		printf("Sale\n");
	}
};

//11.避免自我复制
class SelfCopy
{
public:
	SelfCopy() = default;

	SelfCopy(int* p)
	{
		this->p = p;
	}
	//SelfCopy& operator=(SelfCopy& that)
	//{
	//	if (this == &that)//避免自我复制
	//	{
	//		return;
	//	}
	//	delete this->p;
	//	this->p = new int(*that.p);//深拷贝
	//	return *this;

	//}

	SelfCopy(const SelfCopy& that)
	{
		if(this->p)
			delete this->p;
		this->p = new int(*that.p);
	}

	void swap(SelfCopy& that)
	{
		int* temp = that.p;
		that.p = this->p;
		this->p = temp;
	}

	//使用copy and swap 避免自我复制
	SelfCopy& operator=(SelfCopy& that)
	{
		SelfCopy temp(that);
		this->swap(temp);

		return *this;
	}

	int* p;
};