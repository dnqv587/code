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

	//加密
	string aesCBCEncrypt(string text);

	//解密
	string aesCBCDecrypt(string encStr);

	//获得密钥
	string getKey() { return m_key; }

private:
	
	void initIvec(unsigned char* ivec);//对ivec进行初始化

	string generateRandKey(KeyLen len);//生成随机字符串

private:
	AES_KEY m_encKey;//加密秘钥
	AES_KEY m_decKey;//解密秘钥

	string m_key;
};