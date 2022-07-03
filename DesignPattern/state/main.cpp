#include "state.h"
#include "context.h"

int main(int argc, char** argv)
{
	State* st = new ConcreteStateA;
	Context* con = new Context(st);
	//调用算法
	con->OperationInterface();
	//切换状态
	con->OperationChangeState();
	//调用算法
	con->OperationInterface();
	
	return 0;
}