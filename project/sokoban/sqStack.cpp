#include "sqStack.h"


void initStack(sqStack &s)//初始化栈
{
	s.base = (selemType*)malloc(sizeof(selemType[MAXSIZE]));//申请最大容量为MAXSIZE的空间
	//s.base = new selemType[MAXSIZE];
	if (!s.base)//申请失败
	{
		exit(0);
	}
	s.top = s.base;
	s.stacksize = MAXSIZE;
}

void push(sqStack &s, selemType e)//入栈
{
	if (s.top - s.base == s.stacksize)//栈满
	{
		return;
	}
	*s.top++ = e;//元素e入栈顶，栈顶指针加1
}

void pop(sqStack &s,selemType &e)//出栈
{
	if (s.top == s.base)//栈空
	{
		return;
	}
	 e=*--s.top;
}

selemType getTop(sqStack s)//取栈顶元素
{
	if (s.top!=s.base)//栈不为空
	{
		return *(s.top - 1);
	}
}

bool stackEmpty(sqStack s)//栈是否为空
{
	return (s.top == s.base);
}

void clearStack(sqStack &s)//清为空栈
{
	s.top = s.base;
}

void destroyStack(sqStack& s)
{
	free(s.base);
}