#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <uv.h>

//����
class netbus
{
public:
	static netbus* instance();

	//����tcp�������˿�
	void start_tcp_server(int port);

	//����websocket�������˿�
	void start_ws_server(int port);

	//��ʼ
	void run();
private:
	
	uv_tcp_t* m_tcp;

};