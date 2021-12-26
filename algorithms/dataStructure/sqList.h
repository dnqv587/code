#pragma once
#include <iostream>

using namespace std;

#define ERROR 0
#define OK 1
#define OVERFLOW -2

#define MAXSIZE 1024

typedef int Status;

struct SqList
{
	char* elem;//�洢�ռ�Ļ���ַ
	int length;//��ǰ����
};

class sqList
{
public:
	sqList();
	~sqList();

	//��ձ�
	Status clearList();

	//�ж��Ƿ�Ϊ�ձ�
	bool listEmpty();

	//���ر���Ԫ�صĸ���
	int listLength();

	//��λ����,eΪ��������
	Status getElem(int i, char* e);

	//��ֵ���ң�����λ�ã������ڷ���0
	int locateElem(char e);

	//����ǰ��
	Status priorElem(char cur_e, char* pre_e);

	//���غ��
	Status nextElem(char cur_e, char* next_e);

	//����
	Status listInserte(int i, char e);

	//ɾ��
	Status listDelete(int i);

	//����
	Status traverseList();

private:

	SqList* m_list;
};