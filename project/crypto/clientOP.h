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
	//秘钥协商
	bool secKeyAgree();

	//秘钥校验
	void secKeyCheck();

	//秘钥注销
	void secKeyDelete();

private:

	//解析配置文件
	void analysisConf();

private:
	TcpClient* m_tcp;
	TcpSocket* m_client;
	clientConf m_conf;
};
