#pragma once
#include "session.h"

constexpr int RECV_LEN = 4096;

class session_uv :public session 
{
public:
	//����
	static session* create();
	//����
	void destroy(session* s);

	//�رսӿ�
	virtual void close() override;
	//��������
	virtual void send_data(unsigned char* body, int len) override;
	//��ȡ��ַ
	virtual const char* get_address(int* client_port) override;

public:
	uv_tcp_t tcp_handler;
	char c_address[32];//�ͻ���ip��ַ
	int c_port;//�ͻ���port
	char* recv_buf[RECV_LEN];//���յ�������
	int recved;

	uv_shutdown_t m_shutdown;
	uv_write_t w_req;
	uv_buf_t w_buf;

private:
	void init();
	void exit();

};



