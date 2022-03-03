#include<stdlib.h>
#define MAXSIZE 1000

struct backPoint//回退操作的结构体
{
	int r, c, data;//记录行和列以及对应下标的数据
};

typedef struct preState
{
	struct backPoint pos[3];//记录3个位置的数据
}selemType;

struct sqStack
{
	selemType* base;//栈底指针
	selemType* top;//栈顶指针
	int stacksize;//栈可用的最大容量
};


void initStack(sqStack &s);
void push(sqStack &s, selemType e);
void pop(sqStack &s, selemType &e);
selemType getTop(sqStack s);
bool stackEmpty(sqStack s);
void clearStack(sqStack &s);
void destroyStack(sqStack& s);