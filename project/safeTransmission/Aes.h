#pragma once
#include <iostream>
#include <openssl/aes.h>



enum KeyLen
{
	Key16=16,
	Key24=24,
	Key32=32
};

class AesCrypto
{
public:
	AesCrypto(std::string key);
	AesCrypto(KeyLen len);
	~AesCrypto();

	//����
	std::string aesCBCEncrypt(std::string text);

	//����
	std::string aesCBCDecrypt(std::string encStr);

	//�����Կ
	std::string getKey() { return m_key; }

private:
	
	void initIvec(unsigned char* ivec);//��ivec���г�ʼ��

	std::string generateRandKey(KeyLen len);//��������ַ���

private:
	AES_KEY m_encKey;//������Կ
	AES_KEY m_decKey;//������Կ

	std::string m_key;
};