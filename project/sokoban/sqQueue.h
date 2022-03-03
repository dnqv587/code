#include<stdlib.h>
#define MAXQSIZE 1000//���е���󳤶�

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
	QelemType* base;//�洢�ռ�Ļ���ַ
	int front;//ͷָ��
	int rear;//βָ��
};


void initQueue(sqQueue& Q);
void enQueue(sqQueue& Q, QelemType e);
void deQueue(sqQueue& Q, QelemType& e);
QelemType getHead(sqQueue Q);
void clearQueue(sqQueue& Q);
int queueEmpty(sqQueue Q);
void destroyQueue(sqQueue& Q);