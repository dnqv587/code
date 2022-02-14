#pragma once


class session
{
public:
	//关闭接口
	virtual void close() = 0;
	//发送数据
	virtual void send_data(unsigned char* body, int len) = 0;
	//获取地址
	virtual const char* get_address(int* client_port) = 0;

private:
};
