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

	//������Կ��
	void generateRsaKey(int bits = 1024, std::string path=m_cwd );

	//������Կ�ļ�
	void openRsaKey(const char* publicKeyPath = m_cwd.c_str(), const char* privateKeyPath = m_cwd.c_str());

	//��Կ����
	std::string rsaPubKeyEncrypt(std::string data);
	//��Կ����
	std::string rsaPubKeyDecrypt(std::string encData);

	//˽Կ����
	std::string rsaPriKeyEncrypt(std::string data);
	//˽Կ����
	std::string rsaPriKeyDecrypt(std::string encData);

	//RSAǩ��
	std::string rsaSign(std::string data, Type type);
	//RSAУ��
	bool rsaVerify(std::string data, std::string signData, Type type);

private:
	bool initPublicKey(std::string path);
	
	bool initPrivateKey(std::string path);
private:
	//��Կ
	RSA* m_publicKey;
	RSA* m_privateKey;
	//��Կ����
	int m_priKeySize;
	int m_pubKeySize;
	static std::string m_cwd ;//����·��
};

