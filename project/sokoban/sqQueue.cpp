#include "sqQueue.h"

void initQueue(sqQueue& Q)//初始化队列
{
	//Q.base = new QelemType[MAXQSIZE];//分配一个为MAXQSIZE空间的队列
	Q.base = (QelemType*)malloc(sizeof(QelemType[MAXQSIZE]));
	if (!Q.base) exit(0);//分配空间失败
	Q.front = Q.rear = 0;//队列置为空
}

void enQueue(sqQueue& Q, QelemType e)//入队
{
	if ((Q.rear + 1) % MAXQSIZE == Q.front) return;//队满
	Q.base[Q.rear].child = { e.child.px,e.child.py,e.child.bx, e.child.by }; // 元素e入队
	Q.base[Q.rear].parent = { e.parent.px,e.parent.py,e.parent.bx, e.parent.by };
	Q.rear = (Q.rear + 1) % MAXQSIZE;//队尾指针+1
}

void deQueue(sqQueue& Q, QelemType& e)//出队
{
	if (Q.rear == Q.front) return;//队空
	e = Q.base[Q.front];//保存队头元素
	Q.front = (Q.front + 1) % MAXQSIZE;
}

QelemType getHead(sqQueue Q)//取队头元素
{
	if (Q.front != Q.rear) return Q.base[Q.front];//返回队头的值
}

void clearQueue(sqQueue& Q)//清空队列
{
	Q.front = Q.rear = 0;//队列置为空
}

int queueEmpty(sqQueue Q)//队是否为空
{
	if (Q.front == Q.rear) return 1; else return 0;
}

void destroyQueue(sqQueue& Q)
{
	free(Q.base);
}