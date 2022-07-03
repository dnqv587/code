#include "Context.h"
#include "Strategy.h"


int main(int argc, char** argv)
{
	Strategy* p1 = new ConcreteStrategy1;
	Strategy* p2 = new ConcreteStrategy2;
	Context* c1 = new Context(p1);
	Context* c2 = new Context(p2);
	c1->DoAction();
	c2->DoAction();

	return 0;
}