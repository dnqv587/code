#include "shm.h"

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


