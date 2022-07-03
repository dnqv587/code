#include "Template.h"
#include <stdio.h>
int main(int argc, char** argv)
{
	AbstractClass* p1 = new ConcreteClass1;
	AbstractClass* p2 = new ConcreteClass2;
	p1->TemplateMethod();
	p2->TemplateMethod();

	getchar();
	return 0;
}