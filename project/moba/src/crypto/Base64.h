#pragma once
#include <iostream>
#include <string>
#include <openssl/pem.h>

class Base64
{
public:
	Base64();

	~Base64();

	std::string encode(const std::string data);//����

	char* decode( std::string data);//����

private:
	BIO* m_base64;
	BIO* m_mem;

	char* buf = nullptr;
};