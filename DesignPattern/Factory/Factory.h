#pragma once


class Product;
class Factory
{
public:


private:

};


class Product
{
public:
	virtual ~Product();
	virtual void Operation() = 0;
private:

};

class ConcreteProduct1:public Product
{
public:
	ConcreteProduct1();
	~ConcreteProduct1();

	void Operation() override;

private:

};
