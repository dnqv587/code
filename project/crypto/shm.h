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


class SecKeyShm :public BaseShm
{
public:

	SecKeyShm(int key, int maxNode);
	SecKeyShm(string path, int maxNode);

	~SecKeyShm();

	void shmInit();//��ʼ�������ڴ�

	int shmWrite(NodeSHMInfo* pNodeInfo);//д

	NodeSHMInfo shmRead(string clientID, string serverID);//��

private:
	int m_maxNode;
};
#endif