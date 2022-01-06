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
	//�ն���
	Request() = default;
	//������Ķ������ڽ���
	Request(string encstr);
	//������Ķ������ڱ���
	Request(RequestInfo *info);

	~Request();

	//��ʼ���������
	void initMessage(string encstr);
	//��ʼ���������
	void initMessage(RequestInfo* info);

	//��д�������--->���л��������������л����ַ���
	virtual string encodeMsg() override;
	//��д�������--->�����л����������ؽṹ��/�����
	virtual void* decodeMsg() override;


private:
	//�����л��Ķ���
	string m_encStr;
	//���л��Ķ���
	client::RequestMsg m_Msg;
};