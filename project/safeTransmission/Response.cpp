#include "Response.h"

Response::Response(string encstr)
{
	initMessage(encstr);
}

Response::Response(ResponseInfo* info)
{
	initMessage(info);
}

Response::~Response()
{

}

void Response::initMessage(string encstr)
{
	m_encStr = encstr;
}

void Response::initMessage(ResponseInfo* info)
{
	m_Msg.set_rv(info->status);
	m_Msg.set_clientid(info->clientID);
	m_Msg.set_seckeyid(info->seckeyID);
	m_Msg.set_serverid(info->serverID);
	m_Msg.set_data(info->data);
}

string Response::encodeMsg()
{
	string output;
	m_Msg.SerializeToString(&output);
	return output;
}

void* Response::decodeMsg()
{
	m_Msg.ParseFromString(m_encStr);
	return &m_Msg;
}

