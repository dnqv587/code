#pragma once
#include <string>
#include<stdint.h>
#include <memory>
#include <list>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;


namespace DNQ
{
	class Logger;

	//日志级别
	class LogLevel
	{
	public:
		enum Level
		{
			DEUBG = 1,
			INFO,
			WARN,
			ERROR,
			FATAL
		};

		static const char* ToString(LogLevel::Level level);
	};

	//日志事件
	class LogEvent
	{
	public:
		typedef shared_ptr<LogEvent> ptr;//智能指针

	public:
		LogEvent();

		const char* getFile() const { return m_file; }

		int32_t getLine() const { return m_line; }

		uint32_t getElapse() const { return m_elapse; }

		uint32_t getThreadId() const { return m_threadId; }

		uint32_t getFiberId() const { return m_fiberId; }

		uint64_t getTime() const { return m_time; }

		const string getContent() const { return m_content; }

	private:
		const char* m_file = nullptr;//文件名

		int32_t m_line = 0;//行号
		uint32_t m_elapse;//程序启动开始到现在的毫秒数
		uint32_t m_threadId = 0;//线程号
		uint32_t m_fiberId = 0;//携程号
		uint64_t m_time = 0;//时间戳
		string m_content;//内容
	};

	//日志格式器
	class LogFormatter
	{
	public:
		typedef shared_ptr<LogFormatter> ptr;

		LogFormatter(const string& pattern);

		string format(shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event);//提供给Appender去输出
	private:

		//格式化日志的基类
		class FormatItem
		{
		public:
			typedef shared_ptr<FormatItem> ptr;

			virtual ~FormatItem() {}

			virtual string format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

		};

		//格式化日志内容的子类
		class MessageFormatItem :public LogFormatter::FormatItem
		{
		public:

			virtual string format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//格式化日志等级的子类
		class LevelFormatItem :public LogFormatter::FormatItem
		{
		public:

			virtual string format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;


		};

		//格式化日志启动时间的子类
		class ElapseFormatItem :public LogFormatter::FormatItem
		{
		public:

			virtual string format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//格式化日志名称的子类
		class NameFormatItem :public LogFormatter::FormatItem
		{
		public:

			virtual string format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//格式化线程ID的子类
		class ThreadIdFormatItem :public LogFormatter::FormatItem
		{
		public:

			virtual string format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//格式化本地时间的子类
		class DateTimeFormatItem :public LogFormatter::FormatItem
		{
		public:
			DateTimeFormatItem(const string& format = "%Y:%m%d %H:%M%S");

			virtual string format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

		private:
			string m_format;

		};

		//格式化文件名的子类
		class FileNameFormatItem :public LogFormatter::FormatItem
		{
		public:

			virtual string format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//格式化行号的子类
		class LineFormatItem :public LogFormatter::FormatItem
		{
		public:

			virtual string format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

		};


		/*格式解析：
		%d{%Y-%m-%d %H:%M:%S} : %d 标识输出的是时间 {%Y-%m-%d %H:%M:%S}为时间格式，可选 DateTimeFormatItem
		%T : Tab[\t]            TabFormatItem
		%t : 线程id             ThreadIdFormatItem
		%N : 线程名称           ThreadNameFormatItem
		%F : 协程id             FiberIdFormatItem
		%p : 日志级别           LevelFormatItem
		%c : 日志名称           NameFormatItem
		%f : 文件名             FilenameFormatItem
		%l : 行号               LineFormatItem
		%m : 日志内容           MessageFormatItem
		%n : 换行符[\r\n]       NewLineFormatItem
		*/
		void init();

	private:
		vector<FormatItem> m_items;

		string m_pattern;

	};
	
	//日志输出地
	class LogAppender
	{
	public:
		typedef shared_ptr<LogAppender> ptr;//智能指针

	public:
		virtual ~LogAppender() {}

		virtual void log(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

		void setFormatter(LogFormatter::ptr formatter) { m_formatter = formatter; }

		LogFormatter::ptr getFormatter() const { return m_formatter; }

	protected:
		LogLevel::Level m_level;

		LogFormatter::ptr m_formatter;//日志格式

	private:

	};

	//输出到控制台的Appender
	class StdoutLogAppender :public LogAppender
	{
	public:
		typedef shared_ptr<StdoutLogAppender> ptr;

		virtual void log(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

	private:

	};

	//输出到文件的Appender
	class FileLogAppender :public LogAppender
	{
	public:
		typedef shared_ptr<FileLogAppender> ptr;

		FileLogAppender(const string& filename);

		virtual void log(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

		//冲新打开文件，成功返回true
		bool reOpen();

	private:
		string m_fileName;
		ofstream m_fileStream;
	};

	//日志器
	class Logger
	{
	public:

		typedef shared_ptr<Logger> ptr;
	public:
		Logger();

		Logger(const string& name = "root");

		void log(LogLevel::Level level, LogEvent::ptr event);

		void debug(LogEvent::ptr event);

		void info(LogEvent::ptr event);

		void warn(LogEvent::ptr event);

		void error(LogEvent::ptr event);

		void fatal(LogEvent::ptr event);

		void addAppender(LogAppender::ptr appender);

		void delAppender(LogAppender::ptr appender);

		LogLevel::Level getLevel() const { return m_level; }

		void setLevel(LogLevel::Level level) { m_level = level; }

		string getName() const { return m_name; }

	private:
		string m_name;//日志名称
		LogLevel::Level m_level;//日志级别

		list<LogAppender::ptr> m_appenders;//Appender集合

	};



}