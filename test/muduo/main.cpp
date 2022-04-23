#include <iostream>
#include <string>
#include "designPattern/observer.hpp"

using namespace std;

class Foo : public Observer
{
public:
	~Foo()
	{
		m_Obserberable->unregister(this);
	}

	virtual void update()
	{
		printf("Foo::update() %p\n", this);
	}
	
};

int main(int argc, char* argv[])
{
	Observable o;
	Foo* foo = new Foo;
	foo->observe(&o);
	o.notify();
	delete foo;
	o.notify();
}
