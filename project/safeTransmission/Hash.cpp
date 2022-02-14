#include "Hash.h"

using namespace std;

Hash::Hash(HashType type)
{
	m_type = type;

	switch (m_type)
	{
	case HashType::Hash_MD5:
		MD5_Init(&m_md5);
		break;
	case HashType::Hash_SHA1:
		SHA1_Init(&m_sha1);
		break;
	case HashType::Hash_SHA224:
		SHA224_Init(&m_sha224);
		break;
	case HashType::Hash_SHA256:
		SHA256_Init(&m_sha256);
		break;
	case HashType::Hash_SHA384:
		SHA384_Init(&m_sha384);
		break;
	case HashType::Hash_SHA512:
		SHA512_Init(&m_sha512);
		break;
	default:
		MD5_Init(&m_md5);
		break;
	}
}

Hash::~Hash()
{

}

void Hash::updateData(string data)
{
	switch (m_type)
	{
	case HashType::Hash_MD5:
		MD5_Update(&m_md5,data.c_str(),data.length());
		break;
	case HashType::Hash_SHA1:
		SHA1_Update(&m_sha1, data.c_str(), data.length());
		break;
	case HashType::Hash_SHA224:
		SHA224_Update(&m_sha224, data.c_str(), data.length());
		break;
	case HashType::Hash_SHA256:
		SHA256_Update(&m_sha256, data.c_str(), data.length());
		break;
	case HashType::Hash_SHA384:
		SHA384_Update(&m_sha384, data.c_str(), data.length());
		break;
	case HashType::Hash_SHA512:
		SHA512_Update(&m_sha512, data.c_str(), data.length());
		break;
	default:
		MD5_Update(&m_md5, data.c_str(), data.length());
		break;
	}
}

string Hash::finalData()
{
	string data;
	switch (m_type)
	{
	case HashType::Hash_MD5:
		data = md5Result();
		break;
	case HashType::Hash_SHA1:
		data = sha1Result();
		break;
	case HashType::Hash_SHA224:
		data = sha224Result();
		break;
	case HashType::Hash_SHA256:
		data = sha256Result();
		break;
	case HashType::Hash_SHA384:
		data = sha384Result();
		break;
	case HashType::Hash_SHA512:
		data = sha512Result();
		break;
	default:
		data = md5Result();
		break;
	}
	return data;
}

string Hash::md5Result()
{
	unsigned char md[MD5_DIGEST_LENGTH];
	char res[MD5_DIGEST_LENGTH * 2 + 1];
	MD5_Final(md, &m_md5);
	for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
	{
		sprintf(&res[i * 2], "%02x", md[i]);
	}
	return string(res, MD5_DIGEST_LENGTH * 2 + 1);
}

string Hash::sha1Result()
{
	unsigned char md[SHA_DIGEST_LENGTH];
	char res[SHA_DIGEST_LENGTH * 2 + 1];
	SHA1_Final(md, &m_sha1);
	for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
	{
		sprintf(&res[i * 2], "%02x", md[i]);
	}
	return string(res, SHA_DIGEST_LENGTH * 2 + 1);
}

string Hash::sha224Result()
{
	unsigned char md[SHA224_DIGEST_LENGTH];
	char res[SHA224_DIGEST_LENGTH * 2 + 1];
	SHA224_Final(md, &m_sha224);
	for (int i = 0; i < SHA224_DIGEST_LENGTH; ++i)
	{
		sprintf(&res[i * 2], "%02x", md[i]);
	}
	return string(res, SHA224_DIGEST_LENGTH * 2 + 1);
}

string Hash::sha256Result()
{
	unsigned char md[SHA256_DIGEST_LENGTH];
	char res[SHA256_DIGEST_LENGTH * 2 + 1];
	SHA256_Final(md, &m_sha256);
	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
	{
		sprintf(&res[i * 2], "%02x", md[i]);
	}
	return string(res, SHA256_DIGEST_LENGTH * 2 + 1);
}

string Hash::sha384Result()
{
	unsigned char md[SHA384_DIGEST_LENGTH];
	char res[SHA384_DIGEST_LENGTH * 2 + 1];
	SHA384_Final(md, &m_sha384);
	for (int i = 0; i < SHA384_DIGEST_LENGTH; ++i)
	{
		sprintf(&res[i * 2], "%02x", md[i]);
	}
	return string(res, SHA384_DIGEST_LENGTH * 2 + 1);
}

string Hash::sha512Result()
{
	unsigned char md[SHA512_DIGEST_LENGTH];
	char res[SHA512_DIGEST_LENGTH * 2 + 1];
	SHA512_Final(md, &m_sha512);
	for (int i = 0; i < SHA512_DIGEST_LENGTH; ++i)
	{
		sprintf(&res[i * 2], "%02x", md[i]);
	}
	return string(res, SHA512_DIGEST_LENGTH * 2 + 1);
}
