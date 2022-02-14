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

	//加密
	std::string aesCBCEncrypt(std::string text);

	//解密
	std::string aesCBCDecrypt(std::string encStr);

	//获得密钥
	std::string getKey() { return m_key; }

private:
	
	void initIvec(unsigned char* ivec);//对ivec进行初始化

	std::string generateRandKey(KeyLen len);//生成随机字符串

private:
	AES_KEY m_encKey;//加密秘钥
	AES_KEY m_decKey;//解密秘钥

	std::string m_key;
};