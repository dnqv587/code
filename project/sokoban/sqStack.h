#include<stdlib.h>
#define MAXSIZE 1000

struct backPoint//���˲����Ľṹ��
{
	int r, c, data;//��¼�к����Լ���Ӧ�±������
};

typedef struct preState
{
	struct backPoint pos[3];//��¼3��λ�õ�����
}selemType;

struct sqStack
{
	selemType* base;//ջ��ָ��
	selemType* top;//ջ��ָ��
	int stacksize;//ջ���õ��������
};


void initStack(sqStack &s);
void push(sqStack &s, selemType e);
void pop(sqStack &s, selemType &e);
selemType getTop(sqStack s);
bool stackEmpty(sqStack s);
void clearStack(sqStack &s);
void destroyStack(sqStack& s);