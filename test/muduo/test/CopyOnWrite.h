#pragma once
#include "../base/noncopyable.h"
#include "../thread/mutex.h"
#include <iostream>
#include <map>
#include <string>
#include <memory>


class Customer:public noncopyable
{
public:
	Customer() :m_map(new Map) 
	{
	}

	int quary(const std::string& val)
	{
		MapPtr newPtr = getData();
		Map::const_iterator iter = newPtr->find(val);
		if (iter != newPtr->end())
		{
			return iter->second;
		}
		else
		{
			return -1;
		}
	}

	void update(const std::string str, const int& num)
	{
		MutexLockGuard lock(m_lock);
		if (!m_map.unique())
		{
			m_map.reset(new Map(*m_map));
			std::cout << "copy on update" << std::endl;
		}
		
		m_map->insert(Pair(str, num));
	}

	void destroy(const std::string& val)
	{
		MutexLockGuard lock(m_lock);
		if (!m_map.unique())
		{
			m_map.reset(new Map(*m_map));
			std::cout << "copy on destroy" << std::endl;
		}
		m_map->erase(val);
	}

private:
	typedef std::map<std::string, int> Map;
	typedef std::pair<std::string, int> Pair;
	typedef std::shared_ptr<Map> MapPtr;
	
	MapPtr getData()
	{
		MutexLockGuard lock(m_lock);
		return m_map;
	}

	MapPtr m_map;

	MutexLock m_lock;
};
