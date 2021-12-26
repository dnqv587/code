#include "Rsa.h"


string RsaCrypto::m_cwd = getcwd(NULL, 0);
RsaCrypto::RsaCrypto():m_priKeySize(0), m_pubKeySize(0)
{

	m_privateKey = RSA_new();
	m_publicKey = RSA_new();

}

RsaCrypto::~RsaCrypto()
{
	RSA_free(m_publicKey);
	RSA_free(m_privateKey);
}

void RsaCrypto::generateRsaKey(int bits /*= 1024*/, string path/*=m_pwd */)
{
	RSA* rsa = RSA_new();
	BIGNUM* e = BN_new();
	BN_set_word(e, rand()+10000);

	//生成秘钥对
	RSA_generate_key_ex(rsa, bits, e, NULL);

	//提取秘钥
	m_publicKey = RSAPublicKey_dup(rsa);
	m_privateKey = RSAPrivateKey_dup(rsa);
	//秘钥长度
	m_pubKeySize = RSA_size(m_publicKey);
	m_priKeySize = RSA_size(m_privateKey);

	//秘钥写入文件
	string pubKeyPath = path + "/public.pem";
	string priKeyPath = path + "/private.pem";
	FILE* pubKeyFp = fopen(pubKeyPath.c_str(), "w");
	FILE* PriKeyFp = fopen(priKeyPath.c_str(), "w");

	PEM_write_RSAPublicKey(pubKeyFp, rsa);
	PEM_write_RSAPrivateKey(PriKeyFp, rsa, NULL, NULL, 0, NULL, NULL);

	fclose(PriKeyFp);
	fclose(pubKeyFp);
	RSA_free(rsa);
	BN_free(e);
}

void RsaCrypto::openRsaKey(const char* publicKeyPath /*= m_cwd.c_str()*/, const char* privateKeyPath /*= m_cwd.c_str()*/)
{
	if (publicKeyPath != NULL)
	{
		string pubkeyPath = publicKeyPath;
		if (pubkeyPath == m_cwd)
		{
			pubkeyPath += "/public.pem";
		}
		initPublicKey(pubkeyPath);
		//秘钥长度
		m_pubKeySize = RSA_size(m_publicKey);
	}
	if (privateKeyPath != NULL)
	{
		string prikeyPath = privateKeyPath;
		if (prikeyPath == m_cwd)
		{
			prikeyPath += "/private.pem";
		}
		initPrivateKey(prikeyPath);
		//秘钥长度
		m_priKeySize = RSA_size(m_privateKey);
	}

}

std::string RsaCrypto::rsaPubKeyEncrypt(string data)
{
	//申请存储密文空间
	char* encode = new char[m_pubKeySize + 1];

	if (data.size() > m_pubKeySize)
	{
		cerr << "warrning: data size greater than publicKey size " << endl;
	}
	int ret = RSA_public_encrypt(data.size(), (const unsigned char*)data.c_str(), (unsigned char*)encode, m_publicKey, RSA_PKCS1_PADDING);

	if ( ret>= 0)
	{
		//加密成功
		string retStr = string(encode, ret);
		delete[]encode;
		return retStr;
	}
	delete[]encode;
	cerr << "rsaPubKeyEncrypt error" << endl;
	return  "error";
}

std::string RsaCrypto::rsaPubKeyDecrypt(string encData)
{
	//申请存储密文空间
	char* decode = new char[m_pubKeySize + 1];

	if (encData.size() > m_pubKeySize)
	{
		cerr << "warrning: encode data size greater than publicKey size " << endl;
	}
	int ret = RSA_public_decrypt(encData.size(), (const unsigned char*)encData.c_str(), (unsigned char*)decode, m_publicKey, RSA_PKCS1_PADDING);

	if (ret >= 0)
	{
		//解密成功
		string retStr = string(decode, ret);
		delete[]decode;
		return retStr;
	}
	delete[]decode;
	cerr << "rsaPubKeyDecrypt error" << endl;
	return  "error";
}

std::string RsaCrypto::rsaPriKeyEncrypt(string data)
{
	//申请存储密文空间
	char* encode = new char[m_priKeySize + 1];
	
	if (data.size() > m_priKeySize)
	{
		cerr << "warrning: data size greater than privateKey size " << endl;
	}

	int ret = RSA_private_encrypt(data.size(), (const unsigned char*)data.c_str(), (unsigned char*)encode, m_privateKey, RSA_PKCS1_PADDING);

	if (ret >= 0)
	{
		//加密成功
		string retStr = string(encode, ret);
		delete[]encode;
		return retStr;
	}
	delete[]encode;
	cerr << "rsaPriKeyEncrypt error" << endl;
	return  "error";
}

std::string RsaCrypto::rsaPriKeyDecrypt(string encData)
{
	//申请存储密文空间
	char* decode = new char[m_priKeySize + 1];

	if (encData.size() > m_priKeySize)
	{
		cerr << "warrning:encode data size greater than privateKey size " << endl;
	}
	int ret = RSA_private_decrypt(encData.size(), (const unsigned char*)encData.c_str(), (unsigned char*)decode, m_privateKey, RSA_PKCS1_PADDING);

	if (ret >= 0)
	{
		//解密成功
		string retStr = string(decode, ret);
		delete[]decode;
		return retStr;
	}
	delete[]decode;
	cerr << "rsaPriKeyDecrypt error" << endl;
	return  "error";
}

std::string RsaCrypto::rsaSign(string data, Type type)
{
	unsigned int len;
	string ret;
	//申请存储签名后的数据
	char* sigret = new char[m_priKeySize + 1];

	if (data.size() > m_priKeySize)
	{
		cerr << "warrning: data size greater than privateKey size " << endl;
	}
	//签名
	RSA_sign(type, (const unsigned char*)data.c_str(), data.length(), (unsigned char*)sigret, &len, m_privateKey);

	ret = string(sigret, len);
	delete[]sigret;
	return ret;
}

bool RsaCrypto::rsaVerify(string data, string signData, Type type)
{
	int ret = RSA_verify(type, (const unsigned char*)data.c_str(), data.size(), (const unsigned char*)signData.c_str(), signData.size(), m_publicKey);
	if (ret == 1)
	{
		return true;
	}
	return false;
}

bool RsaCrypto::initPublicKey(string path)
{
	FILE* fp = fopen(path.c_str(), "r");

	if (PEM_read_RSAPublicKey(fp, &m_publicKey, NULL, NULL) == NULL)
	{
		cerr << "read RSAPublicKey error" << endl;
		fclose(fp);
		return false;
	}
	fclose(fp);
	return true;
}

bool RsaCrypto::initPrivateKey(string path)
{
	FILE* fp = fopen(path.c_str(), "r");

	if (PEM_read_RSAPrivateKey(fp, &m_privateKey, NULL, NULL) == NULL)
	{
		cerr << "read RSAPrivateKey error" << endl;
		fclose(fp);
		return false;
	}
	fclose(fp);
	return true;
}

