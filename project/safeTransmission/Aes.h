#pragma once
#include <iostream>
#include <openssl/aes.h>

using namespace std;

enum KeyLen
{
	Key16=16,
	Key24=24,
	Key32=32
};

class AesCrypto
{
public:
	AesCrypto(string key);
	AesCrypto(KeyLen len);
	~AesCrypto();

	//����
	string aesCBCEncrypt(string text);

	//����
	string aesCBCDecrypt(string encStr);

	//�����Կ
	string getKey() { return m_key; }

private:
	
	void initIvec(unsigned char* ivec);//��ivec���г�ʼ��

	string generateRandKey(KeyLen len);//��������ַ���

private:
	AES_KEY m_encKey;//������Կ
	AES_KEY m_decKey;//������Կ

	string m_key;
};