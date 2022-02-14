#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <uv.h>

//单例
class netbus
{
public:
	static netbus* instance();

	//启动tcp服务器端口
	void start_tcp_server(int port);

	//启动websocket服务器端口
	void start_ws_server(int port);

	//开始
	void run();
private:
	
	uv_tcp_t* m_tcp;

};