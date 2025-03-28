#pragma once
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "TcpSocket.h"

using namespace std;

struct clientConf
{
	string clientID;
	string serverID;
	string serverIP;
	unsigned int serverPort;
};

enum cmd
{
	secKeyAgree=1,
	secKeyCheck,
	secKeyDelete,
};

class clientOP
{

public:
	clientOP();
	~clientOP();
	//��ԿЭ��
	bool secKeyAgree();

	//��ԿУ��
	void secKeyCheck();

	//��Կע��
	void secKeyDelete();

private:

	//���������ļ�
	void analysisConf();

private:
	TcpClient* m_tcp;
	TcpSocket* m_client;
	clientConf m_conf;
};
