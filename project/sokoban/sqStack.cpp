#include "sqStack.h"


void initStack(sqStack &s)//��ʼ��ջ
{
	s.base = (selemType*)malloc(sizeof(selemType[MAXSIZE]));//�����������ΪMAXSIZE�Ŀռ�
	//s.base = new selemType[MAXSIZE];
	if (!s.base)//����ʧ��
	{
		exit(0);
	}
	s.top = s.base;
	s.stacksize = MAXSIZE;
}

void push(sqStack &s, selemType e)//��ջ
{
	if (s.top - s.base == s.stacksize)//ջ��
	{
		return;
	}
	*s.top++ = e;//Ԫ��e��ջ����ջ��ָ���1
}

void pop(sqStack &s,selemType &e)//��ջ
{
	if (s.top == s.base)//ջ��
	{
		return;
	}
	 e=*--s.top;
}

selemType getTop(sqStack s)//ȡջ��Ԫ��
{
	if (s.top!=s.base)//ջ��Ϊ��
	{
		return *(s.top - 1);
	}
}

bool stackEmpty(sqStack s)//ջ�Ƿ�Ϊ��
{
	return (s.top == s.base);
}

void clearStack(sqStack &s)//��Ϊ��ջ
{
	s.top = s.base;
}

void destroyStack(sqStack& s)
{
	free(s.base);
}