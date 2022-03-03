#include "sqQueue.h"

void initQueue(sqQueue& Q)//��ʼ������
{
	//Q.base = new QelemType[MAXQSIZE];//����һ��ΪMAXQSIZE�ռ�Ķ���
	Q.base = (QelemType*)malloc(sizeof(QelemType[MAXQSIZE]));
	if (!Q.base) exit(0);//����ռ�ʧ��
	Q.front = Q.rear = 0;//������Ϊ��
}

void enQueue(sqQueue& Q, QelemType e)//���
{
	if ((Q.rear + 1) % MAXQSIZE == Q.front) return;//����
	Q.base[Q.rear].child = { e.child.px,e.child.py,e.child.bx, e.child.by }; // Ԫ��e���
	Q.base[Q.rear].parent = { e.parent.px,e.parent.py,e.parent.bx, e.parent.by };
	Q.rear = (Q.rear + 1) % MAXQSIZE;//��βָ��+1
}

void deQueue(sqQueue& Q, QelemType& e)//����
{
	if (Q.rear == Q.front) return;//�ӿ�
	e = Q.base[Q.front];//�����ͷԪ��
	Q.front = (Q.front + 1) % MAXQSIZE;
}

QelemType getHead(sqQueue Q)//ȡ��ͷԪ��
{
	if (Q.front != Q.rear) return Q.base[Q.front];//���ض�ͷ��ֵ
}

void clearQueue(sqQueue& Q)//��ն���
{
	Q.front = Q.rear = 0;//������Ϊ��
}

int queueEmpty(sqQueue Q)//���Ƿ�Ϊ��
{
	if (Q.front == Q.rear) return 1; else return 0;
}

void destroyQueue(sqQueue& Q)
{
	free(Q.base);
}