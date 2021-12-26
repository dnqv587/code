#include "shm.h"
#ifdef linux
using namespace std;

BaseShm::BaseShm(int key)
{
	getShmID(key,0, 0);
}

BaseShm::BaseShm(int key, int size)
{
	getShmID(key,size, IPC_CREAT | 0664);
}

BaseShm::BaseShm(string path)
{
	key_t key = ftok(path.c_str(), 120);

	getShmID(key, 0, 0);
}

BaseShm::BaseShm(string path, int size)
{
	key_t key = ftok(path.c_str(), 120);

	getShmID(key, size, IPC_CREAT | 0664);
}

BaseShm::~BaseShm()
{

}

void* BaseShm::mapShm()
{
	m_shmAddr = shmat(m_shmID, NULL, 0);

	if (m_shmAddr == (void*)-1)
	{
		return nullptr;
	}
	return m_shmAddr;
}

int BaseShm::unmapShm()
{
	return shmdt(m_shmAddr);
}

int BaseShm::delShm()
{
	return shmctl(m_shmID, IPC_RMID, NULL);
}

int BaseShm::getShmID(key_t key, int shmSize, int flag)
{
	cout << "share memory size: " << shmSize << endl;

	m_shmID = shmget(key, shmSize, flag);
	if (m_shmID == -1)
	{
		cerr << "share memory error" << endl;
	}

	return m_shmID;
}


SecKeyShm::SecKeyShm(int key, int maxNode) : BaseShm(key, maxNode * sizeof(NodeSHMInfo)), m_maxNode(maxNode)
{
}

SecKeyShm::SecKeyShm(string path, int maxNode):BaseShm(path, maxNode * sizeof(NodeSHMInfo)), m_maxNode(maxNode)
{
}

SecKeyShm::~SecKeyShm()
{

}

int SecKeyShm::shmWrite(NodeSHMInfo* pNodeInfo)
{
	int ret = -1;
	// 关联共享内存
	NodeSHMInfo* pAddr = static_cast<NodeSHMInfo*>(mapShm());
	if (pAddr == NULL)
	{
		return ret;
	}

	// 判断传入的网点密钥是否已经存在
	NodeSHMInfo* pNode = NULL;
	for (int i = 0; i < m_maxNode; i++)
	{
		// pNode依次指向每个节点的首地址
		pNode = pAddr + i;
		cout << i << endl;
		cout << "clientID 比较: " << pNode->clientID << ", " << pNodeInfo->clientID << endl;
		cout << "serverID 比较: " << pNode->serverID << ", " << pNodeInfo->serverID << endl;
		cout << endl;
		if (strcmp(pNode->clientID, pNodeInfo->clientID) == 0 &&
			strcmp(pNode->serverID, pNodeInfo->serverID) == 0)
		{
			// 如果找到了该网点秘钥已经存在, 使用新秘钥覆盖旧的值
			memcpy(pNode, pNodeInfo, sizeof(NodeSHMInfo));
			unmapShm();
			cout << "写数据成功: 原数据被覆盖!" << endl;
			return 0;
		}
	}

	// 若没有找到对应的信息, 找一个空节点将秘钥信息写入
	int i = 0;
	NodeSHMInfo tmpNodeInfo; //空结点
	for (i = 0; i < m_maxNode; i++)
	{
		pNode = pAddr + i;
		if (memcmp(&tmpNodeInfo, pNode, sizeof(NodeSHMInfo)) == 0)
		{
			ret = 0;
			memcpy(pNode, pNodeInfo, sizeof(NodeSHMInfo));
			cout << "写数据成功: 在新的节点上添加数据!" << endl;
			break;
		}
	}
	if (i == m_maxNode)
	{
		ret = -1;
	}

	unmapShm();
	return ret;
}

NodeSHMInfo SecKeyShm::shmRead(string clientID, string serverID)
{
	int ret = 0;
	// 关联共享内存
	NodeSHMInfo* pAddr = NULL;
	pAddr = static_cast<NodeSHMInfo*>(mapShm());
	if (pAddr == NULL)
	{
		cout << "共享内存关联失败..." << endl;
		return NodeSHMInfo();
	}
	cout << "共享内存关联成功..." << endl;

	//遍历网点信息
	int i = 0;
	NodeSHMInfo info;
	NodeSHMInfo* pNode = NULL;
	// 通过clientID和serverID查找节点
	cout << "maxNode: " << m_maxNode << endl;
	for (i = 0; i < m_maxNode; i++)
	{
		pNode = pAddr + i;
		cout << i << endl;
		cout << "clientID 比较: " << pNode->clientID << ", " << clientID.data() << endl;
		cout << "serverID 比较: " << pNode->serverID << ", " << serverID.data() << endl;
		if (strcmp(pNode->clientID, clientID.data()) == 0 &&
			strcmp(pNode->serverID, serverID.data()) == 0)
		{
			// 找到的节点信息, 拷贝到传出参数
			info = *pNode;
			cout << "++++++++++++++" << endl;
			cout << info.clientID << " , " << info.serverID << ", "
				<< info.seckeyID << ", " << info.status << ", "
				<< info.seckey << endl;
			cout << "===============" << endl;
			break;
		}
	}

	unmapShm();
	return info;
}
#endif 