#include<stdlib.h>
#define MAXQSIZE 1000//队列的最大长度

struct _child
{
	int px, py, bx, by;
};
struct _parent
{
	int px, py, bx, by;
};

typedef struct position
{
	_child child;
	_parent parent;

}QelemType;

struct sqQueue
{
	QelemType* base;//存储空间的基地址
	int front;//头指针
	int rear;//尾指针
};


void initQueue(sqQueue& Q);
void enQueue(sqQueue& Q, QelemType e);
void deQueue(sqQueue& Q, QelemType& e);
QelemType getHead(sqQueue Q);
void clearQueue(sqQueue& Q);
int queueEmpty(sqQueue Q);
void destroyQueue(sqQueue& Q);