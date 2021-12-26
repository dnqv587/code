#include "sqList.h"

sqList::sqList()
{
	m_list = new SqList[MAXSIZE];
	memset(m_list, 0x00, sizeof(m_list));
	if (!m_list)
		exit(OVERFLOW);
}

sqList::~sqList()
{
	delete[] m_list;
}

Status sqList::getElem(int i, char* e)
{
	if (i<1 || i>m_list->length)
		return ERROR;
	*e = m_list->elem[i - 1];
	return OK;
}

int sqList::locateElem(char e)
{
	for (int i = 0; i < m_list->length; ++i)
	{
		if (m_list->elem[i] == e)
			return i + 1;
	}
	return -1;//²éÕÒÊ§°Ü
}

Status sqList::listInserte(int i, char e)
{
	if (i > m_list->length || i < 1 || m_list->length == MAXSIZE)
		return ERROR;
	for (int i = m_list->length - 1; i >= i - 1; --i)
	{
		m_list->elem[i + 1] = m_list->elem[i];
	}
	m_list->elem[i - 1] = e;
	++m_list->length;
	return OK;
}

Status sqList::listDelete(int i)
{

}

