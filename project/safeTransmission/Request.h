#pragma once
#include <iostream>
#include <string>

#include "./protobuf/client.pb.h"
#include "Codec.h"

using namespace std;

struct RequestInfo
{
	int cmd;
	string clientID;
	string serverID;
	string sign;
	string data;
};

class Request :public Codec
{
public:
	//空对象
	Request() = default;
	//构造出的对象用于解码
	Request(string encstr);
	//构造出的对象用于编码
	Request(RequestInfo *info);

	~Request();

	//初始化解码对象
	void initMessage(string encstr);
	//初始化编码对象
	void initMessage(RequestInfo* info);

	//重写基类对象--->序列化函数，返回序列化的字符串
	virtual string encodeMsg() override;
	//重写基类对象--->反序列化函数，返回结构体/类对象
	virtual void* decodeMsg() override;


private:
	//反序列化的对象
	string m_encStr;
	//序列化的对象
	client::RequestMsg m_Msg;
};