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

	LogEvent::LogEvent(const char* file, int32_t line, uint32_t elapse, uint32_t threadId, uint32_t fiberId, uint64_t time)
		:m_file(file),m_line(line),m_elapse(elapse),m_threadId(threadId),m_fiberId(fiberId),m_time(time),
	{

	}

	Logger::Logger(const string& name /*= "root"*/):m_name(name),m_level(LogLevel::DEUBG)
	{
		m_formatter.reset(new LogFormatter("%d [%p] %f %l %m %n"));
	}

	void Logger::log(LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			for (const auto& c : m_appenders)
			{
				c->log(self,level, event);
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
		if (!appender->getFormatter())
		{
			appender->setFormatter(m_formatter);
		}
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

	void StdoutLogAppender::log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			cout << m_formatter->format(logger, level, event) << endl;
		}
	}

	FileLogAppender::FileLogAppender(const string& filename):m_fileName(filename)
	{

	}

	void FileLogAppender::log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
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
		init();
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

	std::string LogFormatter::MessageFormatItem::format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getContent();
	}

	std::string LogFormatter::LevelFormatItem::format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << LogLevel::ToString(level);
	}

	std::string LogFormatter::ElapseFormatItem::format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getElapse();
	}

	std::string LogFormatter::NameFormatItem::format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << logger->getName();
	}

	std::string LogFormatter::ThreadIdFormatItem::format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getThreadId();
	}


	std::string LogFormatter::FiberIdFormatItem::format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getFiberId() << endl;
	}


	LogFormatter::DateTimeFormatItem::DateTimeFormatItem(const string& format /*= "%Y:%m%d %H:%M%S"*/) :m_format(format)
	{

	}

	std::string LogFormatter::DateTimeFormatItem::format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getTime();
	}

	std::string LogFormatter::FileNameFormatItem::format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getFile();
	}

	std::string LogFormatter::LineFormatItem::format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << event->getLine();
	}

	LogFormatter::StringFormatItem::StringFormatItem(const string& str) :m_string(str)
	{

	}

	std::string LogFormatter::StringFormatItem::format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << m_string;
	}

	std::string LogFormatter::NewLineFormatItem::format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
	{
		os << endl;
	}

	/*
	���ָ�ʽ��
		1:%xxx
		2:%xxx{xxx}
		3:%%
	*/
	void LogFormatter::init()
	{
		
		vector<tuple<string, string, int>> vec;//�洢---�ַ�������ʽ������---
		string nstr;													//---0����ͨ�ַ���  1���ַ�����ʽ 2��
		for (size_t i = 0; i < m_pattern.size(); ++i)
		{
			if (m_pattern[i] != '%')
			{
				nstr.append(1, m_pattern[i]);// %��ǰ����Ӵ�
				continue;
			}
			if ((i + 1) < m_pattern.size())
			{
				if (m_pattern[i + 1] == '%')
				{
					nstr.append(1, '%');//ת��%
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
				if (!isalpha(m_pattern[n] && m_pattern[n] != '{' && m_pattern[n] != '}'))
				{
					break;
				}
				if (fmt_status == 0)
				{
					if (m_pattern[n] == '{')
					{
						str = m_pattern.substr(i + 1, n - i - 1);//��ȡ % �� { ���Ӵ�
						fmt_status = 1;//������ʽ
						fmt_begin = n;
						++n;
						continue;
					}
					
				}
				if (fmt_status == 1)
				{
					if (m_pattern[n] == '}')
					{
						fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin);//��ȡ{ �� } ���Ӵ�
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
					vec.push_back(make_tuple(nstr, string(), 0));
					nstr.clear();
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
				if (!nstr.empty())
				{
					vec.push_back(make_tuple(nstr, "", 0));
					nstr.clear();
				}
				vec.push_back(make_tuple(str, fmt, 1));
				i = n;
			}	
		}
		if (!nstr.empty())
		{
			vec.push_back(make_tuple(nstr, "", 0));
		}

		static map<string, function< FormatItem::ptr(const string& str)> > s_format_items =
		{
	#define XX(str,C) \
			{#str,[](const string fmt) {return FormatItem::ptr(new C(fmt)); }}

			XX(m,MessageFormatItem),
			XX(p,LevelFormatItem),
			XX(r,ElapseFormatItem),
			XX(c,NameFormatItem),
			XX(t,ThreadIdFormatItem),
			XX(n,NewLineFormatItem),
			XX(d,DateTimeFormatItem),
			XX(f,FileNameFormatItem),
			XX(l,LineFormatItem),
	#undef XX
		};

		for (auto& i : vec)
		{
			if(get<2>(i)==0)
			{
				m_items.push_back(FormatItem::ptr(new StringFormatItem(get<0>(i))));//��ͨ�ַ���

			}
			else
			{
				auto iter = s_format_items.find(get<0>(i));
				if (iter == s_format_items.end())
				{
					m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + get<0>(i) + ">>")));//��������
				}
				else
				{
					m_items.push_back(iter->second(get<1>(i)));
				}
			}

			cout << "(" << get<0>(i) << ") - " << "(" << get<1>(i) << ") - " << "(" << get<2>(i) << ")" << endl;
		}
	}





}