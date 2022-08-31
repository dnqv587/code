#pragma once
#include <stdio.h>
#include <string>
#include <utility>
#include <exception>
#include <stdlib.h>
#include <assert.h>
#include <string>

#include <fstream>

//----------virtual析构测试
class Vbase
{
public:
	Vbase()
	{
		printf("constructor Vbase!\n");
	}
	/*
	* 1、使用virtual析构函数，在使用基类指针指向派生类对象时，如果不声明为virtual，则delete不会调用派生类的析构函数
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


//9、----------绝不在构造和析构过程中调用virtual函数

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

//11.--------避免自我复制
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

//----------异常处理
class MyException :public std::exception
{
public:
	MyException()
		:std::exception("this is MyException", 1)
	{

	}

};

//--------隐式类型转换
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


//18-19 --------接口设计
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

//pass by const referrence or pass by value
class Foo
{
public:
	Foo() = default;
	Foo(int val)
		:_a(val)
	{

	}
	~Foo() = default;

	Foo& instansce()
	{
		return *this;
	}

	void setVal(int val)
	{
		_a = val;
	}

	int getVal()
	{
		return _a;
	}
private:
	int _a;
};

//----------operator测试
namespace Algo
{
	class Math
	{
	public:
		Math() = delete;
		Math(int val)
			:_val(val)
		{

		}
		
		Math& operator+(const Math& that)
		{
			this->_val += that._val;
			return *this;
		}

		Math& operator-(const Math& that)
		{
			this->_val -= that._val;
			return *this;
		}

		Math& operator*(const Math& that)
		{
			this->_val *= that._val;
			return *this;
		}
		Math& operator/(const Math& that)
		{
			this->_val /= that._val;
			return *this;
		}

		Math& operator+(int val)
		{
			this->_val += val;
			return *this;
		}

		Math& operator-(int val)
		{
			this->_val -= val;
			return *this;
		}

		Math& operator*(int val)
		{
			this->_val *= val;
			return *this;
		}
		Math& operator/(int val)
		{
			this->_val /= val;
			return *this;
		}

		operator int()
		{
			return this->_val;
		}

		void print()
		{
			printf("Algo::Math:_val:%d\n", this->_val);
		}

	private:
		int _val;
	};

	Math& add( Math& lhs, const Math& rhs)
	{
		return lhs + rhs;
	}
}

//----------------不抛出异常的swap
//swap的前提是，对象支持拷贝构造或拷贝操作符
class Widget1
{
public:
	Widget1(int v, Foo* f)
		:_val(v),
		_foo(f)
	{

	}
	~Widget1()
	{
		delete _foo;
	}

	Widget1(const Widget1& that)
	{
		this->_val = that._val;
		this->_foo = new Foo;
		*this->_foo = *that._foo;
	}
	Widget1& operator=(const Widget1& that)
	{
		if (&that == this)
		{
			return *this;
		}
		this->_val = that._val;
		this->_foo = new Foo;
		*this->_foo = *that._foo;
	}
	Foo* getFoo()
	{
		return _foo;
	}
	int getVal()
	{
		return _val;
	}
	//内部swap,不能抛出异常
	void swap(Widget1& that) noexcept
	{
		std::swap(this->_foo, that._foo);//调用std::swap
		std::swap(this->_val, that._val);
	}

private:
	int _val;
	Foo* _foo;
};

//重载特例化std::swap
/*
* 1、C++只允许对class template偏特化，在function templates身上偏特化行不通
* 2、不可以添加新的classes或functions到std里面
* 错误：template<typename T>//std::swap一个重载版本
		1、void swap<Widget<T> >(Widget<T>& a,Widget<T>& b)
		2、void swap(Widget<T>& a,Widget<T>& b)
*/
namespace std
{
	template<>
	void swap<Widget1>(Widget1& lhs, Widget1& rhs) noexcept //这是std::swap针对T是Widget1的特换版本
	{
		lhs.swap(rhs);//调用内部函数
	}
}

//派生类中调用基类函数
class Base1
{
public:
	virtual void print()
	{
		std::cout << "this is Base1' print\n";
	}
};

class Impl1:public Base1
{
public:
	void print() override
	{
		//static_cast<Base1>(*this).print();//调用基类中的print
		Base1::print();//调用基类中的print

		std::cout << "this is Impl1\n";
	}
};

class Impl2 :public Base1
{
public:
	void print() override
	{
		std::cout << "this is Impl2\n";
	}
};


//声明和实现分离
class API
{
public:
	API(const char* name)
		:_name(name)
	{

	}

	virtual const char* api() const = 0;

	virtual ~API()
	{
		printf("API destructor\n");
	}

protected:
	const char* _name;
};

class IMPL :public API
{
public:
	IMPL(const char* name)
		:API(name),
		_name("IMPL")
	{

	}
	const char* api() const
	{
		return API::_name;
	}

	~IMPL()
	{
		printf("IMPL destructor\n");
	}
private:
	const char* _name;
};

class Combi
{
public:

	Combi(API* api)
		:_api(api)
	{

	}
	void print()
	{
		printf("%s\n",_api->api());
	}

private:
	std::shared_ptr<API> _api;//API为纯虚函数，所以不能直接实例化，只能使用pointer或reference
};