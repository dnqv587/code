#pragma once
#include "session.h"

constexpr int RECV_LEN = 4096;

class session_uv :public session 
{
public:
	//创建
	static session* create();
	//销毁
	void destroy(session* s);

	//关闭接口
	virtual void close() override;
	//发送数据
	virtual void send_data(unsigned char* body, int len) override;
	//获取地址
	virtual const char* get_address(int* client_port) override;

public:
	uv_tcp_t tcp_handler;
	char c_address[32];//客户端ip地址
	int c_port;//客户端port
	char* recv_buf[RECV_LEN];//接收到的数据
	int recved;

	uv_shutdown_t m_shutdown;
	uv_write_t w_req;
	uv_buf_t w_buf;

private:
	void init();
	void exit();

};



