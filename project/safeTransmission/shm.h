#pragma once
#ifdef linux
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>


using namespace std;

class NodeSHMInfo
{
public:
	NodeSHMInfo() : status(0), seckeyID(0)
	{
		memset(clientID, 0x00, sizeof(clientID));
		memset(serverID, 0x00, sizeof(serverID));
		memset(seckey, 0x00, sizeof(seckey));
	}
	int status;		// 秘钥状态: 1可用, 0:不可用
	int seckeyID;	// 秘钥的编号
	char clientID[12];	// 客户端ID, 客户端的标识
	char serverID[12];	// 服务器ID, 服务器标识
	char seckey[128];	// 对称加密的秘钥
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


class SecKeyShm :public BaseShm
{
public:

	SecKeyShm(int key, int maxNode);
	SecKeyShm(string path, int maxNode);

	~SecKeyShm();

	void shmInit();//初始化共享内存

	int shmWrite(NodeSHMInfo* pNodeInfo);//写

	NodeSHMInfo shmRead(string clientID, string serverID);//读

private:
	int m_maxNode;
};
#endif