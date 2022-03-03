#include "serverOP.h"

#include <fstream>
#include <unistd.h>
#include <sstream>
#include <json/json.h>
#include <pthread.h>

#include "Hash.h"
#include "./protobuf/client.pb.h"

using namespace Json;

serverOP::serverOP()
{
	string path = getcwd(NULL, 0);
	string file = path + "/conf/serverConf.json";
	ifstream in(file);

	Value root;
	Reader read;
	read.parse(in, root);

	m_conf.clientID = root["clientID"].asString();
	m_conf.serverID = root["serverID"].asString();
	m_conf.clientIP = root["clientIP"].asString();
	m_conf.serverPort = root["serverPort"].asInt();


	in.close();
}

serverOP::~serverOP()
{
	delete m_server;
}

void serverOP::startServer()
{
	m_server = new TcpServer(m_conf.serverPort);

	while (1)
	{ 
		cout << "等待客户端连接..." << endl;
		TcpSocket* tcp = m_server->TcpCreate();
		if (m_server == NULL)
		{
			continue;
		}
		cout << "客户端连接成功" << endl;
		//创建子线程
		pthread_t th;
		pthread_create(&th, NULL, working_thread, this);
		m_list.insert(make_pair(th, tcp));

	}


}

bool serverOP::secKeyAgree(client::RequestMsg* str,pthread_t mapKey)
{
	bool ret;
	string path = getcwd(NULL, 0);
	path += "/conf/serverPublic.pem";
	//将公钥写入磁盘
	ofstream out(path);
	out << str->data();

	ResponseInfo info;

	//签名校验
	RsaCrypto rsa;
	//rsa.openRsaKey(path.c_str(), NULL);
	Hash sha256(Hash_SHA256);
	sha256.updateData(str->data());
	if (!rsa.rsaVerify(sha256.finalData(),str->sign(),Type_SHA256))
	{
		cout << "签名校验失败..." << endl;
		info.status = false;
		ret = false;
	}
	else
	{
		//生成对称加密密钥
		AesCrypto aes(Key32);
		string secKey = rsa.rsaPubKeyEncrypt(aes.getKey());

		//序列化数据
		info.clientID = m_conf.clientID;
		info.serverID = m_conf.serverID;
		info.seckeyID = 12;//需要数据库操作
		info.status = true;
		info.data = secKey;

		ret = true;
	}

	CodecFactory* enFac = new RespondFactory(&info);
	Codec* enCode = enFac->createCodec();
	string encStr = enCode->encodeMsg();

	//发送数据
	m_list[mapKey]->TcpSend(encStr, 10);

	delete enFac;
	delete enCode;
	return ret;
}

void serverOP::secKeyCheck()
{

}

void serverOP::secKeyDelete()
{

}

void* serverOP::working_thread(void* arg)
{
	serverOP* op = (serverOP*)arg;
	TcpSocket* tcp = op->m_list[pthread_self()];

	//反序列化数据
	string recRequest = tcp->TcpRecv();
	CodecFactory* deFac = new RequestFactory(recRequest);
	Codec* deCode = deFac->createCodec();
	client::RequestMsg* decStr = (client::RequestMsg*)deCode->decodeMsg();

	switch (decStr->cmdtype())
	{
	case 1:
		//秘钥协商
		op->secKeyAgree(decStr, pthread_self());
		break;
	case 2:
		//秘钥校验
		op->secKeyCheck();
		break;
	case 3:
		//秘钥注销
		op->secKeyDelete();
		break;
	case 4:
		//秘钥查看
		break;
	}

	//释放资源
	op->m_list.erase(pthread_self());
	delete tcp;
	delete deFac;
	delete deCode;

	return NULL;

}
