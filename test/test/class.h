#pragma once
#include <stdio.h>
#include <string>
#include <utility>
#include <exception>
#include <stdlib.h>
#include <assert.h>
#include <string>

#include <fstream>

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
	//		return *this;
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

//异常处理
class MyException :public std::exception
{
public:
	MyException()
		:std::exception("this is MyException", 1)
	{

	}

};

//隐式类型转换
class Person
{
public:
	Person() = default;
	Person(const char* name, int age)
		:_name(name),
		_age(age)
	{

	}
	Person(const Person& that)
		:_age(that._age),
		_name(that._name)
	{
		printf("Person copy constructor\n");
	}

	const char* _name;
	int _age;
};

class PersonWrapper
{
public:
	PersonWrapper() = default;

	PersonWrapper(Person& person)
		:_person(person)
	{

	}

	PersonWrapper(const PersonWrapper& that)
	{
		this->_person = that._person;
		printf("PersonWrapper copy constructor\n");
	}

	operator Person()
	{
		return _person;

	}
private:
	Person _person;
};


//18-19 接口设计
struct Year
{
	Year() = default;
	explicit Year(int val)
		:Y(val)
	{
		assert(Y > 1970 && Y < 2100);
	}
	Year(const Year& y)
		:Y(y.Y)
	{
		assert(Y > 1970 && Y < 2100);
	}

	operator int()
	{
		return Y;
	}

	int Y;
};

struct Month
{
	Month() = default;
	explicit Month(int val)
		:M(val)
	{
		assert(M > 0 && M < 13);
	}
	Month(const Month& m)
		:M(m.M)
	{
		assert(M > 0 && M < 13);
	}

	operator int()
	{
		return M;
	}

	int M;
};

struct Day
{
	Day() = default;
	explicit Day(int val)
		:D(val)
	{
		if (D == 1 || D == 3 || D == 5 || D == 7 || D == 8 || D == 10 || D == 12)
		{
			assert(D > 0 && D <= 31);
		}
		else
		{
			if (D == 2)
			{
				assert(D > 0 && D <= 29);
			}
			assert(D > 0 && D <= 30);
		}
	}

	Day(const Day& d)
		:D(d.D)
	{
		if (D == 1 || D == 3 || D == 5 || D == 7 || D == 8 || D == 10 || D == 12)
		{
			assert(D > 0 && D <= 31);
		}
		else
		{
			assert(D > 0 && D <= 30);
		}

	}

	operator int()
	{
		return D;
	}

	int D;
};

class Date
{
public:
	explicit Date(Year Y, Month M, Day D)
		:_Y(Y),
		_M(M),
		_D(D)
	{

	}

	Date(int y, int m, int d)
		:_Y(Year (y)),
		_M(Month (m)),
		_D(Day (d))
	{

	}

	std::string format()
	{
		return "Year:" + std::to_string(_Y.Y) + " Month:" + std::to_string(_M.M) + " Day:" + std::to_string(_D.D);
	}

	Year year()
	{
		return _Y;
	}

	Month month()
	{
		return _M;
	}

	Day day()
	{
		return _D;
	}

private:
	Year _Y;
	Month _M;
	Day _D;
};