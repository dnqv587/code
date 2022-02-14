#include "Aes.h"
#include <time.h>

using namespace std;

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

AesCrypto::AesCrypto(KeyLen len)
{
	string key = generateRandKey(len);
	AES_set_decrypt_key((const unsigned char*)key.c_str(), key.size() * 8, &m_decKey);
	AES_set_encrypt_key((const unsigned char*)key.c_str(), key.size() * 8, &m_encKey);
	m_key = key;
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

string AesCrypto::generateRandKey(KeyLen len)
{
	int flag = 0;
	string ret;
	srand((unsigned int)time(NULL));

	string cs = "~!@#$%^&*()_+-=/*-+[]{}|;:',<.>/?";

	for (int i = 0; i < len; ++i)
	{
		flag =  rand()% 4;

		switch (flag)
		{
		case 0:// a-z
			ret.append(1, 'a' + rand() % 26);
			break;
		case 1:// A-Z
			ret.append(1, 'A' + rand() % 26);
			break;
		case 2:// 0-9
			ret.append(1, '0' + rand() % 10);
			break;
		case 3:// 特殊字符
			ret.append(1, cs[rand() % cs.length()]);
			break;
		}

	}

	return ret;
}

void AesCrypto::initIvec(unsigned char* ivec)
{
	for (int i = 0; i < AES_BLOCK_SIZE; ++i)
	{
		ivec[i] = m_key.at(AES_BLOCK_SIZE - i - 1);
	}
}
