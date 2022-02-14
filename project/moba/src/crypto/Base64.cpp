#include "Base64.h"

Base64::Base64()
{
	m_mem = BIO_new(BIO_s_mem());
	m_base64 = BIO_new(BIO_f_base64());
	BIO_push(m_base64, m_mem);
	BIO_set_flags(m_base64, BIO_FLAGS_BASE64_NO_NL);
}

Base64::~Base64()
{
	BIO_free_all(m_base64);
	if(buf!=nullptr)
		delete[] buf;
}

std::string Base64::encode(const std::string data)
{
	// 写数据
	BIO_write(m_base64, data.c_str(), data.size()+1);
	BIO_flush(m_base64);
	// 得到内存对象指针
	BUF_MEM* memPtr;
	BIO_get_mem_ptr(m_base64, &memPtr);

	return std::string(memPtr->data, memPtr->length );
}

char* Base64::decode(std::string data)
{	

	BIO_write(m_mem, data.c_str(), data.size());
	BIO_flush(m_mem);

	if (buf != nullptr)
	{
		delete[] buf;
		buf = nullptr;
	}
	buf = new char[data.size()];

	BIO_read(m_base64, buf, data.size());
	//std::string str = std::string(buf, sizeof(buf));
	//delete[] buf;
	return buf;

}
