#pragma once
#include <iostream>
#include <string>
#include <openssl/md5.h>
#include <openssl/sha.h>

using namespace std;

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

	void updateData(string data);//Ìí¼ÓÊý¾Ý

private:
	HashType m_type;

	MD5_CTX m_md5;
	SHA_CTX m_sha1;
	SHA256_CTX m_sha224;
	SHA256_CTX m_sha256;
	SHA512_CTX m_sha384;
	SHA512_CTX m_sha512;
};