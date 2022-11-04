#pragma once
#include <stdio.h>

//原型模式-prototype pattern

//原型类，声明一个克隆自己的接口
class Animal
{
public:
	//克隆接口，纯虚函数，子类需要实现
	virtual Animal* Clone() = 0;

	virtual void print() = 0;
};

class Cat :Animal
{
public:
	Cat(const char* name)
		:m_name(name)
	{

	}
	~Cat() {}

	Cat(const Cat& cat)
	{
		m_name = cat.m_name;
	}

	void test()
	{
		printf("Cat Test\n");
	}

	Animal* Clone() override
	{
		return new Cat(*this);
	}

	void print() override
	{
		printf("this is Cat\n");
	}

private:
	const char* m_name;
};

class Dog :Animal
{
public:
	Dog(const char* name)
		:m_name(name)
	{

	}
	~Dog() {}

	Dog(const Dog& cat)
	{
		m_name = cat.m_name;
	}

	void test()
	{
		printf("Dog Test\n");
	}

	Animal* Clone() override
	{
		return new Dog(*this);
	}

	void print() override
	{
		printf("this is Dog\n");
	}

private:
	const char* m_name;
};