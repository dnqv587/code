#include "Request.h"

Request::Request(string encstr)
{
	initMessage(encstr);
}

Request::Request(RequestInfo* info)
{
	initMessage(info);
}

Request::~Request()
{

}

void Request::initMessage(string encstr)
{
	m_encStr = encstr;
}

void Request::initMessage(RequestInfo* info)
{
	m_Msg.set_cmdtype(info->cmd);
	m_Msg.set_clientid(info->clientID);
	m_Msg.set_serverid(info->serverID);
	m_Msg.set_sign(info->sign);
	m_Msg.set_data(info->data);
}

string Request::encodeMsg()
{
	string output;
	m_Msg.SerializeToString(&output);
	return output;
}

void* Request::decodeMsg()
{
	m_Msg.ParseFromString(m_encStr);
	return &m_Msg;
}

