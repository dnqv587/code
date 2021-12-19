#include "Factory.h"

using namespace std;

CodecFactory::CodecFactory() = default;

CodecFactory::~CodecFactory()
{

}

Codec* CodecFactory::createCodec()
{
	return nullptr;
}

RequestFactory::RequestFactory(string enc)
{
	m_flag = true;
	m_encStr = enc;
}

RequestFactory::RequestFactory(RequestInfo* info)
{
	m_flag = false;
	m_info = info;
}

RequestFactory::~RequestFactory()
{

}

Codec* RequestFactory::createCodec()
{
	if(m_flag)
		return new Request(m_encStr);
	else
		return new Request(m_info);
}



RespondFactory::RespondFactory(string enc)
{
	m_flag = true;
	m_encStr = enc;
}

RespondFactory::RespondFactory(ResponseInfo* info)
{
	m_flag = false;
	m_info = info;
}

RespondFactory::~RespondFactory()
{

}

Codec* RespondFactory::createCodec()
{
	if(m_flag)
		return new Response(m_encStr);
	else
		return new Response(m_info);
}
