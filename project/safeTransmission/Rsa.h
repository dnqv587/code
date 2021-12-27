#pragma once
#include <iostream>
#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <unistd.h>




using namespace std;

enum Type
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
	void generateRsaKey(int bits = 1024, string path=m_cwd );

	//读入秘钥文件
	void openRsaKey(const char* publicKeyPath = m_cwd.c_str(), const char* privateKeyPath = m_cwd.c_str());

	//公钥加密
	string rsaPubKeyEncrypt(string data);
	//公钥解密
	string rsaPubKeyDecrypt(string encData);

	//私钥加密
	string rsaPriKeyEncrypt(string data);
	//私钥解密
	string rsaPriKeyDecrypt(string encData);

	//RSA签名
	string rsaSign(string data, Type type);
	//RSA校验
	bool rsaVerify(string data, string signData, Type type);

private:
	bool initPublicKey(string path);
	
	bool initPrivateKey(string path);
private:
	//秘钥
	RSA* m_publicKey;
	RSA* m_privateKey;
	//秘钥长度
	int m_priKeySize;
	int m_pubKeySize;
	static string m_cwd ;//工作路径
};

