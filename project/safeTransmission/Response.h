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
	//�ն���
	Response() = default;
	//������Ķ������ڽ���ĳ���
	Response(string encstr);
	//����Ķ������ڱ���ĳ���
	Response(ResponseInfo *info);

	~Response();

	//init�������ڿչ���������ʹ��
	void initMessage(string encstr);
	//����ʹ��
	void initMessage(ResponseInfo* info);

	//��д���ຯ��--->���л�����
	virtual string encodeMsg() override;
	//��д���ຯ��--->�����л����������ؽṹ�����
	virtual void* decodeMsg() override;
	

private:
	string m_encStr;
	service::RespondMsg m_Msg;
};
