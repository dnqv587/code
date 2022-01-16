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
#include <map>
#include <functional>

using namespace std;


namespace DNQ
{
	class Logger;

	//��־����
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

	


	//��־�¼�
	class LogEvent
	{
	public:
		typedef shared_ptr<LogEvent> ptr;//����ָ��

	public:
		LogEvent(const char* file,int32_t line,uint32_t elpase,uint32_t threadId,uint32_t fiberId,uint64_t time);

		const char* getFile() const { return m_file; }

		int32_t getLine() const { return m_line; }

		uint32_t getElapse() const { return m_elapse; }

		uint32_t getThreadId() const { return m_threadId; }

		uint32_t getFiberId() const { return m_fiberId; }

		uint64_t getTime() const { return m_time; }

		string getContent() const { return m_ss.str(); }

		stringstream& getSS() { return m_ss; }

	private:
		const char* m_file = nullptr;//�ļ���

		int32_t m_line = 0;//�к�
		uint32_t m_elapse;//����������ʼ�����ڵĺ�����
		uint32_t m_threadId = 0;//�̺߳�
		uint32_t m_fiberId = 0;//Я�̺�
		uint64_t m_time = 0;//ʱ���
		stringstream m_ss;//����
	};


	



	//��־��ʽ��
	class LogFormatter
	{
	public:
		typedef shared_ptr<LogFormatter> ptr;

		LogFormatter(const string& pattern);

		string format(shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event);//�ṩ��Appenderȥ���
	public:

		//��ʽ����־�Ļ���
		class FormatItem
		{
		public:
			typedef shared_ptr<FormatItem> ptr;

			//FormatItem(const string& fmt = "") {}

			virtual ~FormatItem() {}

			virtual string format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

		};

		//��ʽ����־���ݵ�����
		class MessageFormatItem :public LogFormatter::FormatItem
		{
		public:
			MessageFormatItem(const string& str = "") {}

			 string format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//��ʽ����־�ȼ�������
		class LevelFormatItem :public LogFormatter::FormatItem
		{
		public:
			LevelFormatItem(const string& str = "") {}
			 string format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;


		};

		//��ʽ����־����ʱ�������
		class ElapseFormatItem :public LogFormatter::FormatItem
		{
		public:
			ElapseFormatItem(const string& str = "") {}

			 string format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//��ʽ����־���Ƶ�����
		class NameFormatItem :public LogFormatter::FormatItem
		{
		public:
			NameFormatItem(const string& str = "") {}

			 string format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//��ʽ���߳�ID������
		class ThreadIdFormatItem :public LogFormatter::FormatItem
		{
		public:
			ThreadIdFormatItem(const string& str = "") {}

			 string format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//��ʽ��Я��ID������
		class FiberIdFormatItem :public LogFormatter::FormatItem
		{
		public:
			FiberIdFormatItem(const string& str = "") {}

			 string format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//��ʽ������ʱ�������
		class DateTimeFormatItem :public LogFormatter::FormatItem
		{
		public:
			DateTimeFormatItem(const string& format = "%Y:%m%d %H:%M%S");

			 string format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

		private:
			string m_format;

		};

		//��ʽ���ļ���������
		class FileNameFormatItem :public LogFormatter::FormatItem
		{
		public:
			FileNameFormatItem(const string& str = "") {}

			string format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//��ʽ���кŵ�����
		class LineFormatItem :public LogFormatter::FormatItem
		{
		public:
			LineFormatItem(const string& str = "") {}

			 string format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//���е�����
		class NewLineFormatItem :public LogFormatter::FormatItem
		{
		public:
			NewLineFormatItem(const string& str = "") {}

			 string format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

		};

		//��ʽ���ַ���������
		class StringFormatItem :public LogFormatter::FormatItem
		{
		public:
			StringFormatItem(const string& str);

			 string format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
		private:
			string m_string;
		};


		/*��ʽ������
		%d{%Y-%m-%d %H:%M:%S} : %d ��ʶ�������ʱ�� {%Y-%m-%d %H:%M:%S}Ϊʱ���ʽ����ѡ DateTimeFormatItem
		%T : Tab[\t]            TabFormatItem
		%t : �߳�id             ThreadIdFormatItem
		%N : �߳�����           ThreadNameFormatItem
		%F : Э��id             FiberIdFormatItem
		%p : ��־����           LevelFormatItem
		%c : ��־����           NameFormatItem
		%f : �ļ���             FilenameFormatItem
		%l : �к�               LineFormatItem
		%m : ��־����           MessageFormatItem
		%n : ���з�[\r\n]       NewLineFormatItem
		*/
		void init();

	private:
		vector<FormatItem::ptr> m_items;

		string m_pattern;

	};
	
	//��־�����
	class LogAppender
	{
	public:
		typedef shared_ptr<LogAppender> ptr;//����ָ��

	public:
		virtual ~LogAppender() {}

		virtual void log(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

		void setFormatter(LogFormatter::ptr formatter) { m_formatter = formatter; }

		LogFormatter::ptr getFormatter() const { return m_formatter; }

	protected:
		LogLevel::Level m_level;

		LogFormatter::ptr m_formatter;//��־��ʽ

	private:

	};

	//���������̨��Appender
	class StdoutLogAppender :public LogAppender
	{
	public:
		typedef shared_ptr<StdoutLogAppender> ptr;

		virtual void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

	private:

	};

	//������ļ���Appender
	class FileLogAppender :public LogAppender
	{
	public:
		typedef shared_ptr<FileLogAppender> ptr;

		FileLogAppender(const string& filename);

		virtual void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

		//���´��ļ����ɹ�����true
		bool reOpen();

	private:
		string m_fileName;
		ofstream m_fileStream;
	};

	
	//��־��
	class Logger :public enable_shared_from_this<Logger>//��ȡ�Լ���ָ��
	{
	public:

		typedef shared_ptr<Logger> ptr;
	public:
		Logger() {};

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
		string m_name;//��־����
		LogLevel::Level m_level;//��־����

		list<LogAppender::ptr> m_appenders;//Appender����

		LogFormatter::ptr m_formatter;

	};


}