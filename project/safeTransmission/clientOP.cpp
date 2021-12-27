# include "clientOP.h"

#include <string.h>
#include <unistd.h>
#include <fstream>
#include <json/json.h>
#include <sstream>

#include "Factory.h"
#include "Request.h"
#include "Rsa.h"


clientOP::clientOP()
{
	analysisConf();
}

clientOP::~clientOP()
{

}

bool clientOP::secKeyAgree()
{
	string path = getcwd(NULL, 0);
	path += "/conf";

	RsaCrypto* key = new RsaCrypto;
	key->generateRsaKey(1024, path);
	//读公钥文件
	string file = path + "/public.pem";
	ifstream in(file);
	stringstream str;
	str << in.rdbuf();

	string sign = key->rsaSign(str.str(), Type_SHA256);

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

	//释放资源
	delete enFac;
	delete reqEncode;
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

	m_conf.clientID = root["slientID"].asString();
	m_conf.serverID = root["serverID"].asString();
	m_conf.serverIP = root["serverIP"].asString();
	m_conf.serverPort = root["serverPort"].asInt();

	in.close();
}
