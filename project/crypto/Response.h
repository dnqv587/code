#pragma once
#include <iostream>
#include <string>

#include "./protobuf/service.pb.h"

#include "Codec.h"

using namespace std;

struct ResponseInfo
{
	int status;
	int seckeyID;
	string clientID;
	string serverID;
	string data;
};

class Response :public Codec
{
public:
	//空对象
	Response() = default;
	//构造出的对象用于解码的场景
	Response(string encstr);
	//构造的对象用于编码的场景
	Response(ResponseInfo *info);

	~Response();

	//init函数用于空构造对象解码使用
	void initMessage(string encstr);
	//编码使用
	void initMessage(ResponseInfo* info);

	//重写基类函数--->序列化函数
	virtual string encodeMsg() override;
	//重写基类函数--->反序列化函数，返回结构体对象
	virtual void* decodeMsg() override;
	

private:
	string m_encStr;
	service::RespondMsg m_Msg;
};
