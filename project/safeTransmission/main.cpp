#include <iostream>

#include "Request.h"
#include "Response.h"
#include "Factory.h"
#include "TcpSocket.h"
#include "Rsa.h"
#include "Aes.h"
#include "Hash.h"

#include <json/json.h>
#include <fstream>;

using namespace std;
using namespace Json;

extern "C"
{
#include <openssl/applink.c>
}

//编码
string encodeMsg(Codec* codec)//父类指针指向子类对象
{
	return codec->encodeMsg();
}
//解码
void* decodeMsg(Codec* codec)
{
	return codec->decodeMsg();
}

void test()
{
	RequestInfo reqInfo;
	reqInfo.cmd = 9;
	reqInfo.clientID = "you";
	reqInfo.serverID = "me";
	reqInfo.data = "你好";
	reqInfo.sign = "world";


	//编码
	//Request reqEncode(&reqInfo);
	//工厂生产一个对象
	CodecFactory* enFac = new RequestFactory(&reqInfo);
	Codec* reqEncode = enFac->createCodec();

	//string reqMsg = reqEncode.encodeMsg();
	string reqMsg = encodeMsg(reqEncode);//使用多态；条件：1.继承关系  2.在子类中重写父类虚函数  3.父类指针指向子类对象
	//解码
	//Request reqDecode(reqMsg);
	CodecFactory* deFac = new RequestFactory(reqMsg);
	Codec* reqDecode = deFac->createCodec();

	//client::RequestMsg* requestMsg = (client::RequestMsg*)reqDecode.decodeMsg();
	client::RequestMsg* requestMsg = (client::RequestMsg*)decodeMsg(reqDecode);

	cout << "cmd:" << requestMsg->cmdtype() << " clientID:" << requestMsg->clientid()
		<< " serverID:" << requestMsg->serverid() << " data:" << requestMsg->data() << " sign:" << requestMsg->sign() << endl;

	delete enFac;
	delete deFac;
}

void tcptest()
{
	TcpClient* client = new TcpClient;
	TcpSocket* cfd = client->TcpConnectPoll("192.168.80.1", 8888, 10000);
	cfd->TcpSend("hello!");
	while (1)
	{
		cfd->TcpSend("hello!");
		cfd->TcpRecv(10000);
		//cout << cfd->get_data() << endl;
	}
}

void aestest()
{
	//准备数据
	const char* str = "AES是一套对称密钥的密码术，目前已广泛使用，用于替代已经不够安全的DES算法。所谓对称密钥，就是说加密和解密用的是同一个密钥，消息的发送方和接收方在消息传递前需要享有这个密钥。和非对称密钥体系不同，这里的密钥是双方保密的，不会让任何第三方知道。";
	//准备秘钥
	const char* key = "1234567891234567";
	//初始化秘钥
	AES_KEY enckey;
	AES_set_encrypt_key((const unsigned char *)key, 128, &enckey);
	AES_KEY deckey;
	AES_set_decrypt_key((const unsigned char*)key, 128, &deckey);
	//加密
	int length = strlen((const char*)str) + 1;
	if (length % 16 != 0)
	{
		length = ((length / 16) + 1) * 16;
	}
	unsigned char* out = new unsigned char[length];
	unsigned char ivec[AES_BLOCK_SIZE];
	memset(ivec, 0x00, sizeof(ivec));
	AES_cbc_encrypt((const unsigned char*)str, out, length, &enckey, ivec, AES_ENCRYPT);
	//解密
	memset(ivec, 0x00, sizeof(ivec));
	unsigned char* data = new unsigned char[length];
	AES_cbc_encrypt(out, data, length, &deckey, ivec, AES_DECRYPT);
	//打印
	cout << "解密：" << data << endl;

}

void rsaclass()
{
	RsaCrypto* rsa = new RsaCrypto;

	//rsa->generateRsaKey(4096);

	rsa->openRsaKey();
	string str = "123456789";
	string encode=rsa->rsaPubKeyEncrypt(str);
	cout << rsa->rsaPriKeyDecrypt(encode) << endl;

	string sh = rsa->rsaSign(str, Type_SHA256);
	cout<<rsa->rsaVerify("123456789", sh, Type_SHA256)<<endl;
}

void aesclass()
{
	AesCrypto* aes = new AesCrypto("1234567891234567");
	string code = aes->aesCBCEncrypt("123456789111111111111111111111");
	cout << aes->aesCBCDecrypt(code) << endl;
}

void hashclass()
{
	Hash* hash = new Hash(Hash_MD5);
	hash->updateData("123456789");
	cout << hash->finalData() << endl;
}

void jsonwrite()
{
	//[12, 13.45, "hello, world", true, false, [1,2,"aa"], {"a":"b"}]
	//组织数据->放入Value对象中
	Value json;//最外层的json数组
	//json数组添加元素
	json.append(12);//隐式类型转换->json.append(Value(12));
	json.append(13.45);
	json.append("hello,world");
	json.append(true);
	json.append(false);
	//创建数组
	Value sub;
	sub.append(1);
	sub.append(2);
	sub.append("aa");
	json.append(sub);//往json数组添加sub数组
	//创建object对象->在value中
	Value obj;
	obj["a"] = "b";
	json.append(obj);
	//将value对象格式化->string 
	//string jsonText = json.toStyledString();
	FastWriter w;
	string jsonText = w.write(json);
	//写磁盘文件
	ofstream ofs("test.json");
	ofs << jsonText;
	ofs.close();
}

void jsonread()
{
	//打开磁盘文件
	ifstream ifs("test.json");
	Reader r;
	Value json;
	//读取json数据
	r.parse(ifs, json);
	if (json.isArray())//如果是数组
	{
		//遍历数组
		for (int i = 0; i < json.size(); ++i)
		{
			Value sub = json[i];
			if (sub.isInt())
			{
				cout << sub.asInt() << " ";
			}
			else if (sub.isString())
			{
				cout << sub.asString() << " ";
			}
			else if (sub.isDouble())
			{
				cout << sub.asDouble() << " ";
			}
			else if (sub.isBool())
			{
				cout << sub.asBool() << " ";
			}
			else if (sub.isObject())
			{
				cout << sub["a"].asString() << endl;
			}
		}
	}

}

int main(int argc, char* argv[])
{
#ifdef WIN32
	//配置windows socket版本
	WORD wVersionRequested;
	WSADATA WSAData;
	wVersionRequested = MAKEWORD(2, 2);
	int ret = WSAStartup(wVersionRequested, &WSAData);
	if (ret != 0)
	{
		perror("WSAStartup error");
		return -1;
	}
#endif

	//test();
	//tcptest();

	//aestest();
	//rsaclass();
	//aesclass();
	//hashclass();
	//jsonwrite();
	jsonread();

	system("pause");
	return 0;

} 