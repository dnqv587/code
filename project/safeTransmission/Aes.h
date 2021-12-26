#pragma once
#include <iostream>
#include <openssl/aes.h>

using namespace std;

class AesCrypto
{
public:
	AesCrypto(string key);
	~AesCrypto();

	//����
	string aesCBCEncrypt(string text);

	//����
	string aesCBCDecrypt(string encStr);

private:
	
	void initIvec(unsigned char* ivec);//��ivec���г�ʼ��

private:
	AES_KEY m_encKey;//������Կ
	AES_KEY m_decKey;//������Կ

	string m_key;
};