#pragma once
#include <iostream>
#include <string>

#include "Codec.h"
#include "Request.h"
#include "Response.h"

using namespace std;

class CodecFactory
{
public:
	CodecFactory();
	~CodecFactory();
	virtual Codec* createCodec();
};

class RequestFactory :public CodecFactory
{
public:
	RequestFactory(string enc);
	RequestFactory(RequestInfo* info);
	~RequestFactory();

	virtual Codec* createCodec() override;

private:
	bool m_flag;
	string m_encStr;
	RequestInfo* m_info;
};

class RespondFactory :public CodecFactory
{
public:
	RespondFactory(string enc);
	RespondFactory(ResponseInfo* info);
	~RespondFactory();

	virtual Codec* createCodec() override;

private:
	bool m_flag;
	string m_encStr;
	ResponseInfo* m_info;
};