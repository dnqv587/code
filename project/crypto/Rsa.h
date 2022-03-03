#pragma once
#include <iostream>
#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#ifdef linux
#include <unistd.h>
#else
#include <direct.h>
#endif


enum  Type
{
	Type_MD5 = NID_md5,
	Type_SHA1 = NID_sha1,
	Type_SHA224 = NID_sha224,
	Type_SHA256 = NID_sha256,
	Type_SHA384 = NID_sha384,
	Type_SHA512 = NID_sha512
};



class RsaCrypto
{
public:
	RsaCrypto();
	~RsaCrypto();

	//生成密钥对
	void generateRsaKey(int bits = 1024, std::string path=m_cwd );

	//读入秘钥文件
	void openRsaKey(const char* publicKeyPath = m_cwd.c_str(), const char* privateKeyPath = m_cwd.c_str());

	//公钥加密
	std::string rsaPubKeyEncrypt(std::string data);
	//公钥解密
	std::string rsaPubKeyDecrypt(std::string encData);

	//私钥加密
	std::string rsaPriKeyEncrypt(std::string data);
	//私钥解密
	std::string rsaPriKeyDecrypt(std::string encData);

	//RSA签名
	std::string rsaSign(std::string data, Type type);
	//RSA校验
	bool rsaVerify(std::string data, std::string signData, Type type);

private:
	bool initPublicKey(std::string path);
	
	bool initPrivateKey(std::string path);
private:
	//秘钥
	RSA* m_publicKey;
	RSA* m_privateKey;
	//秘钥长度
	int m_priKeySize;
	int m_pubKeySize;
	static std::string m_cwd ;//工作路径
};

