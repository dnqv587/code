#include <iostream>

#include "Request.h"
#include "Response.h"
#include "Factory.h"
#include "TcpSocket.h"


using namespace std;

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
		cout << cfd->get_data() << endl;
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
	tcptest();
	system("pause");
	return 0;

} 