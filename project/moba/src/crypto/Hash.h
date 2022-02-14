#pragma once
#include <iostream>
#include <string>
#include <openssl/md5.h>
#include <openssl/sha.h>


enum HashType
{
	Hash_MD5,
	Hash_SHA1,
	Hash_SHA224,
	Hash_SHA256,
	Hash_SHA384,
	Hash_SHA512
};

class Hash
{
public:
	Hash(HashType type);
	~Hash();

	void updateData(std::string data);//添加数据

	std::string finalData();//生成hash

private:
	std::string md5Result();
	std::string sha1Result();
	std::string sha224Result();
	std::string sha256Result();
	std::string sha384Result();
	std::string sha512Result();

private:
	HashType m_type;

	MD5_CTX m_md5;
	SHA_CTX m_sha1;
	SHA256_CTX m_sha224;
	SHA256_CTX m_sha256;
	SHA512_CTX m_sha384;
	SHA512_CTX m_sha512;
};