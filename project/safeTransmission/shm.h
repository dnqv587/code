#pragma once
#include <iostream>

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

using namespace std;

struct NodeSHMInfo
{
	int status;
	int seckeyID;
	string clientID;
	string serverID;
	string seckey;
	string pubkey;
};

class BaseShm
{
public:
	BaseShm(int key);//通过key来打开共享内存
	BaseShm(int key,int size);//通过key来创建/打开共享内存
	BaseShm(string path);//通过路径来打开内存
	BaseShm(string path, int size);//通过路径来创建/打开内存

	~BaseShm();

	//将当前进程和共享内存关联
	void* mapShm();

	//接触关联
	int unmapShm();

	//删除共享内存
	int delShm();


protected:

	void* m_shmAddr;//共享内存首地址

private:

	int getShmID(key_t key, int shmSize, int flag);
private:

	int m_shmID;//共享内存ID

};


class SecKeyShm:public BaseShm
{
public:

	SecKeyShm(int key,int maxNode);
	SecKeyShm(string path, int maxNode);

	~SecKeyShm();

	int shmWrite(NodeSHMInfo* pNodeInfo);

	int shmRead(string clientID, string serverID, NodeSHMInfo pNodeInfo);

private:
	int m_maxNode;
};