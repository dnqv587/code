#pragma once
#include <iostream>
#include <string>
#include <pthread.h>
#include <map>

#include "TcpSocket.h"
#include "Request.h"
#include "Aes.h"
#include "Factory.h"
#include "Rsa.h"
#include "Response.h"


using namespace std;


struct serverConf
{
	string clientID;
	string serverID;
	string clientIP;
	unsigned int serverPort;
};

class serverOP
{
public:
	serverOP();
	~serverOP();

	//����������
	void startServer();

	//��ԿЭ��
	bool secKeyAgree(client::RequestMsg* str, pthread_t mapKey);

	//��ԿУ��
	void secKeyCheck();

	//��Կע��
	void secKeyDelete();

	//�̹߳����߳�
	static void* working_thread(void* arg);//����ص���������ľ�̬����(�Ƽ�)�������Ԫ��������ͨ����

private:
	serverConf m_conf;

	TcpServer* m_server = nullptr;

	map<pthread_t, TcpSocket*> m_list;//����ͻ��˶���
};