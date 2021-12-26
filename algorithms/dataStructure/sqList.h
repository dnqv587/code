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
	char* elem;//存储空间的基地址
	int length;//当前长度
};

class sqList
{
public:
	sqList();
	~sqList();

	//清空表
	Status clearList();

	//判断是否为空表
	bool listEmpty();

	//返回表中元素的个数
	int listLength();

	//按位查找,e为传出参数
	Status getElem(int i, char* e);

	//按值查找，返回位置，不存在返回0
	int locateElem(char e);

	//返回前驱
	Status priorElem(char cur_e, char* pre_e);

	//返回后继
	Status nextElem(char cur_e, char* next_e);

	//插入
	Status listInserte(int i, char e);

	//删除
	Status listDelete(int i);

	//遍历
	Status traverseList();

private:

	SqList* m_list;
};