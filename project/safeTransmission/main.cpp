#include <iostream>

#include "Request.h"
#include "Response.h"
#include "Factory.h"
#include "TcpSocket.h"

#include <openssl/aes.h>
using namespace std;

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

	aestest();

	system("pause");
	return 0;

} 