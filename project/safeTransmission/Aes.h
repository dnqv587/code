#pragma once
#include <iostream>
#include <openssl/aes.h>

using namespace std;

class AesCrypto
{
public:
	AesCrypto(string key);
	~AesCrypto();

	//加密
	string aesCBCEncrypt(string text);

	//解密
	string aesCBCDecrypt(string encStr);

private:
	
	void initIvec(unsigned char* ivec);//对ivec进行初始化

private:
	AES_KEY m_encKey;//加密秘钥
	AES_KEY m_decKey;//解密秘钥

	string m_key;
};