# include "clientOP.h"

#include <string.h>
#include <unistd.h>
#include <fstream>
#include <json/json.h>
#include <sstream>

#include "Factory.h"
#include "Request.h"
#include "Rsa.h"
#include "Response.h"
#include "Hash.h"

using namespace Json;

#define MAX_KEY_SIZE 512

clientOP::clientOP()
{
	analysisConf();
	m_tcp = new TcpClient;
	m_client = m_tcp->TcpConnect(m_conf.serverIP, m_conf.serverPort, 10);
	
}

clientOP::~clientOP()
{
	delete m_client;
	delete m_tcp;
} 

bool clientOP::secKeyAgree()
{
	string path = getcwd(NULL, 0);
	path += "/conf";

	RsaCrypto* key = new RsaCrypto;
	key->generateRsaKey(MAX_KEY_SIZE, path);
	//读公钥文件
	string file = path + "/public.pem";
	ifstream in(file);
	stringstream str;
	str << in.rdbuf();

	Hash sha256(Hash_SHA256);
	sha256.updateData(str.str());
	string sign = key->rsaSign(sha256.finalData(), Type_SHA256);//对公钥的哈希值进行签名

	RequestInfo request;
	request.clientID = m_conf.clientID;
	request.serverID = m_conf.serverID;
	request.cmd = cmd::secKeyAgree;//秘钥协商
	request.data = str.str();
	request.sign = sign;
	//序列化数据
	CodecFactory* enFac = new RequestFactory(&request);
	Codec* reqEncode = enFac->createCodec();
	string encstr = reqEncode->encodeMsg();

	//发送数据
	m_client->TcpSend(encstr);
	//等待服务器回复
	string msg = m_client->TcpRecv();
	//解析服务器数据-->反序列化
	CodecFactory* deFac = new RespondFactory(msg);
	Codec* resDecode = deFac->createCodec();
	ResponseInfo* decstr;
	decstr = (ResponseInfo*)resDecode->decodeMsg();
	if (decstr->status)
	{
		cout << "秘钥协商失败" << endl;
		return false;
	}
	//将得到的密文解密
	string aesKey = key->rsaPriKeyDecrypt(decstr->data);
	cout << "对称加密秘钥" << aesKey << endl;

	//释放资源
	delete enFac;
	delete reqEncode;
	delete deFac;
	delete resDecode;
}

void clientOP::secKeyCheck()
{

}

void clientOP::analysisConf()
{
	//解析配置文件
	string path = getcwd(NULL, 0);
	path += "/conf/clientConf.json";
	ifstream in(path);

	Value root;
	Reader read;
	read.parse(in, root);

	m_conf.clientID = root["clientID"].asString();
	m_conf.serverID = root["serverID"].asString();
	m_conf.serverIP = root["serverIP"].asString();
	m_conf.serverPort = root["serverPort"].asInt();

	in.close();
}
