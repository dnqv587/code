#include "Hash.h"


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
		MD5_Final((unsigned char *)data.c_str(),&m_md5);
		break;
	case HashType::Hash_SHA1:
		SHA1_Final((unsigned char*)data.c_str(), &m_sha1);
		break;
	case HashType::Hash_SHA224:
		SHA224_Final((unsigned char*)data.c_str(), &m_sha224);
		break;
	case HashType::Hash_SHA256:
		SHA256_Final((unsigned char*)data.c_str(), &m_sha256);
		break;
	case HashType::Hash_SHA384:
		SHA384_Final((unsigned char*)data.c_str(), &m_sha384);
		break;
	case HashType::Hash_SHA512:
		SHA512_Final((unsigned char*)data.c_str(), &m_sha512);
		break;
	default:
		MD5_Final((unsigned char*)data.c_str(), &m_md5);
		break;
	}
	return data;
}

