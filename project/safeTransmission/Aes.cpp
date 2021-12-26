#include "Aes.h"

AesCrypto::AesCrypto(string key)
{
	if (key.size() == 16 || key.size() == 24 || key.size() == 32)
	{
		AES_set_decrypt_key((const unsigned char*)key.c_str(), key.size() * 8, &m_decKey);
		AES_set_encrypt_key((const unsigned char*)key.c_str(), key.size() * 8, &m_encKey);
		m_key = key;
	}
	else
	{
		cerr << "key size should be 16 24 32 byte" << endl;
	}
}

AesCrypto::~AesCrypto()
{

}

string AesCrypto::aesCBCEncrypt(string text)
{
	string ret;

	int len = ((((text.size() + 1) / 16) + 1) * 16);//长度必须为16的整数倍

	char* encode = new char[len];

	unsigned char ivec[AES_BLOCK_SIZE];
	initIvec(ivec);

	AES_cbc_encrypt((const unsigned char*)text.c_str(), (unsigned char*)encode, len, &m_encKey, ivec, AES_ENCRYPT);

	ret = string(encode);
	delete[] encode;
	return ret;
}

string AesCrypto::aesCBCDecrypt(string encStr)
{
	string ret;

	int len = ((((encStr.size() + 1) / 16) + 1) * 16);//长度必须为16的整数倍

	char* decode = new char[len];

	unsigned char ivec[AES_BLOCK_SIZE];
	initIvec(ivec);

	AES_cbc_encrypt((const unsigned char*)encStr.c_str(), (unsigned char*)decode, len, &m_decKey, ivec, AES_DECRYPT);

	ret = string(decode);
	delete[] decode;
	return ret;
}

void AesCrypto::initIvec(unsigned char* ivec)
{
	for (int i = 0; i < AES_BLOCK_SIZE; ++i)
	{
		ivec[i] = m_key.at(AES_BLOCK_SIZE - i - 1);
	}
}
