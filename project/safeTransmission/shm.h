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
	BaseShm(int key);//ͨ��key���򿪹����ڴ�
	BaseShm(int key,int size);//ͨ��key������/�򿪹����ڴ�
	BaseShm(string path);//ͨ��·�������ڴ�
	BaseShm(string path, int size);//ͨ��·��������/���ڴ�

	~BaseShm();

	//����ǰ���̺͹����ڴ����
	void* mapShm();

	//�Ӵ�����
	int unmapShm();

	//ɾ�������ڴ�
	int delShm();


protected:

	void* m_shmAddr;//�����ڴ��׵�ַ

private:

	int getShmID(key_t key, int shmSize, int flag);
private:

	int m_shmID;//�����ڴ�ID

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