#pragma once
#include <iostream>

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

using namespace std;

class NodeSHMInfo
{
public:
	NodeSHMInfo() : status(0), seckeyID(0)
	{
		bzero(clientID, sizeof(clientID));
		bzero(serverID, sizeof(serverID));
		bzero(seckey, sizeof(seckey));
	}
	int status;		// ��Կ״̬: 1����, 0:������
	int seckeyID;	// ��Կ�ı��
	char clientID[12];	// �ͻ���ID, �ͻ��˵ı�ʶ
	char serverID[12];	// ������ID, ��������ʶ
	char seckey[128];	// �ԳƼ��ܵ���Կ
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