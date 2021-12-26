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

//����
string encodeMsg(Codec* codec)//����ָ��ָ���������
{
	return codec->encodeMsg();
}
//����
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
	reqInfo.data = "���";
	reqInfo.sign = "world";


	//����
	//Request reqEncode(&reqInfo);
	//��������һ������
	CodecFactory* enFac = new RequestFactory(&reqInfo);
	Codec* reqEncode = enFac->createCodec();

	//string reqMsg = reqEncode.encodeMsg();
	string reqMsg = encodeMsg(reqEncode);//ʹ�ö�̬��������1.�̳й�ϵ  2.����������д�����麯��  3.����ָ��ָ���������
	//����
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
	//׼������
	const char* str = "AES��һ�׶Գ���Կ����������Ŀǰ�ѹ㷺ʹ�ã���������Ѿ�������ȫ��DES�㷨����ν�Գ���Կ������˵���ܺͽ����õ���ͬһ����Կ����Ϣ�ķ��ͷ��ͽ��շ�����Ϣ����ǰ��Ҫ���������Կ���ͷǶԳ���Կ��ϵ��ͬ���������Կ��˫�����ܵģ��������κε�����֪����";
	//׼����Կ
	const char* key = "1234567891234567";
	//��ʼ����Կ
	AES_KEY enckey;
	AES_set_encrypt_key((const unsigned char *)key, 128, &enckey);
	AES_KEY deckey;
	AES_set_decrypt_key((const unsigned char*)key, 128, &deckey);
	//����
	int length = strlen((const char*)str) + 1;
	if (length % 16 != 0)
	{
		length = ((length / 16) + 1) * 16;
	}
	unsigned char* out = new unsigned char[length];
	unsigned char ivec[AES_BLOCK_SIZE];
	memset(ivec, 0x00, sizeof(ivec));
	AES_cbc_encrypt((const unsigned char*)str, out, length, &enckey, ivec, AES_ENCRYPT);
	//����
	memset(ivec, 0x00, sizeof(ivec));
	unsigned char* data = new unsigned char[length];
	AES_cbc_encrypt(out, data, length, &deckey, ivec, AES_DECRYPT);
	//��ӡ
	cout << "���ܣ�" << data << endl;

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
	//��֯����->����Value������
	Value json;//������json����
	//json�������Ԫ��
	json.append(12);//��ʽ����ת��->json.append(Value(12));
	json.append(13.45);
	json.append("hello,world");
	json.append(true);
	json.append(false);
	//��������
	Value sub;
	sub.append(1);
	sub.append(2);
	sub.append("aa");
	json.append(sub);//��json�������sub����
	//����object����->��value��
	Value obj;
	obj["a"] = "b";
	json.append(obj);
	//��value�����ʽ��->string 
	//string jsonText = json.toStyledString();
	FastWriter w;
	string jsonText = w.write(json);
	//д�����ļ�
	ofstream ofs("test.json");
	ofs << jsonText;
	ofs.close();
}

void jsonread()
{
	//�򿪴����ļ�
	ifstream ifs("test.json");
	Reader r;
	Value json;
	//��ȡjson����
	r.parse(ifs, json);
	if (json.isArray())//���������
	{
		//��������
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
	//����windows socket�汾
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