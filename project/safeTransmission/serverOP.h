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

	//启动服务器
	void startServer();

	//秘钥协商
	bool secKeyAgree(client::RequestMsg* str, pthread_t mapKey);

	//秘钥校验
	void secKeyCheck();

	//秘钥注销
	void secKeyDelete();

	//线程工作线程
	static void* working_thread(void* arg);//这个回调可以是类的静态函数(推荐)，类的友元函数，普通函数

private:
	serverConf m_conf;

	TcpServer* m_server = nullptr;

	map<pthread_t, TcpSocket*> m_list;//保存客户端对象
};