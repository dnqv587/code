#include "DLog.h"

namespace DNQ
{
	const char* LogLevel::ToString(LogLevel::Level level)
	{
		switch (level)
		{
		case LogLevel::DEUBG:
			return "DEUBG";
			break;
		case LogLevel::INFO:
			return "INFO";
			break;
		case LogLevel::WARN:
			return "WARN";
			break;
		case LogLevel::ERROR:
			return "ERROR";
			break;
		case LogLevel::FATAL:
			return "FATAL";
			break;
		default:
			return "UNKNOW";
		}
	}

	Logger::Logger(const string& name /*= "root"*/):m_name(name)
	{

	}

	void Logger::log(LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			for (const auto& c : m_appenders)
			{
				c->log(level, event);
			}
		}
	}

	void Logger::debug(LogEvent::ptr event)
	{
		log(LogLevel::DEUBG, event);
	}

	void Logger::info(LogEvent::ptr event)
	{
		log(LogLevel::INFO, event);
	}

	void Logger::warn(LogEvent::ptr event)
	{
		log(LogLevel::WARN, event);
	}

	void Logger::error(LogEvent::ptr event)
	{
		log(LogLevel::ERROR, event);
	}

	void Logger::fatal(LogEvent::ptr event)
	{
		log(LogLevel::FATAL, event);
	}

	void Logger::addAppender(LogAppender::ptr appender)
	{
		m_appenders.push_back(appender);
	}

	void Logger::delAppender(LogAppender::ptr appender)
	{
		for (auto iter = m_appenders.begin(); iter != m_appenders.end(); ++iter)
		{
			if (*iter == appender)
			{
				m_appenders.erase(iter);
				break;
			}
		}
	}

	void StdoutLogAppender::log(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			cout << m_formatter->format(logger, level, event) << endl;
		}
	}

	FileLogAppender::FileLogAppender(const string& filename):m_fileName(filename)
	{

	}

	void FileLogAppender::log(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			m_fileStream << m_formatter->format(logger, level, event);
		}
	}

	bool FileLogAppender::reOpen()
	{
		if (m_fileStream)
		{
			m_fileStream.close();
			m_fileStream.open(m_fileName);
			return !!m_fileStream;
		}
	}

	LogFormatter::LogFormatter(const string& pattern):m_pattern(pattern)
	{

	}

	std::string LogFormatter::format(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		stringstream ss;
		for (auto& c : m_items)
		{
			c.format(ss, logger,level, event);
		}
		return ss.str();
	}

	/*
	三种格式：
		1:%xxx
		2:%xxx{xxx}
		3:%%
	*/
	void LogFormatter::init()
	{
		//str,format,type
		vector<tuple<string, string, int>> vec;
		string nstr;
		for (size_t i = 0; i < m_pattern.size(); ++i)
		{
			if (m_pattern[i] != '%')
			{
				nstr.append(1, m_pattern[i]);// %号前面的子串
				continue;
			}
			if ((i + 1) < m_pattern.size())
			{
				if (m_pattern[i + 1] == '%')
				{
					nstr.append(1, '%');//真%
					continue;
				}
			}
			size_t n = i + 1;
			int fmt_status = 0;
			size_t fmt_begin = 0;

			string str;
			string fmt;
			while (n < m_pattern.size())
			{
				if (isspace(m_pattern[n]))
				{
					break;
				}
				if (fmt_status == 0)
				{
					if (m_pattern[n] == '{')
					{
						str = m_pattern.substr(i + 1, n - i - 1);//截取 % 到 { 的子串
						fmt_status = 1;//解析格式
						fmt_begin = n;
						++n;
						continue;
					}
					
				}
				if (fmt_status == 1)
				{
					if (m_pattern[n] == '}')
					{
						fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin);//截取{ 到 } 的子串
						fmt_status = 2;
						++n;
						break;
					}
				}
				++n;
			}
			if (fmt_status == 0)
			{
				if (!nstr.empty())
				{
					vec.push_back(make_tuple(nstr, "", 0));
				}
				str = m_pattern.substr(i + 1, n - i - 1);
				vec.push_back(make_tuple(str, fmt, 1));
				i = n;
			}
			else if (fmt_status == 1)
			{
				cout << "pattern parse error:" << m_pattern << " - " << m_pattern.substr(i) << endl;
				vec.push_back(make_tuple("<pattern error>", fmt, 1));
			}
			else if (fmt_status == 2)
			{
				vec.push_back(make_tuple(str, fmt, 1));
				i = n;
			}	
		}
		if (!nstr.empty())
		{
			vec.push_back(make_tuple(nstr, "", 0));
		}
	}

	std::string LogFormatter::MessageFormatItem::format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getContent();
	}

	std::string LogFormatter::LevelFormatItem::format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << LogLevel::ToString(level);
	}

	std::string LogFormatter::ElapseFormatItem::format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getElapse();
	}

	std::string LogFormatter::NameFormatItem::format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << logger->getName();
	}

	std::string LogFormatter::ThreadIdFormatItem::format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getThreadId();
	}


	LogFormatter::DateTimeFormatItem::DateTimeFormatItem(const string& format /*= "%Y:%m%d %H:%M%S"*/):m_format(format)
	{

	}

	std::string LogFormatter::DateTimeFormatItem::format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getTime();
	}

	std::string LogFormatter::FileNameFormatItem::format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getFile();
	}

	std::string LogFormatter::LineFormatItem::format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getLine();
	}

}