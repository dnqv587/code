#pragma once


class session
{
public:
	//�رսӿ�
	virtual void close() = 0;
	//��������
	virtual void send_data(unsigned char* body, int len) = 0;
	//��ȡ��ַ
	virtual const char* get_address(int* client_port) = 0;

private:
};
