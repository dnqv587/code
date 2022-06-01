#include "logging.h"
#include "../thread/thread.h"

__thread char t_errnobuf[512];//错误信息
__thread char t_time[64];
__thread time_t t_lastSecond;

Logger::LogLevel initLogLevel()
{
	if (::getenv("LOG_TRACE"))
		return Logger::TRACE;
	else if (::getenv("LOG_DEBUG"))
		return Logger::DEBUG;
	else
		return Logger::INFO;
}

//默认日志输出回调---输出到stdout
void defaultOutput(const char* msg, int len)
{
	::fwrite(msg, 1, len, stdout);
}
//默认日志刷新回调--刷新stdout
void defaultFlush()
{
	::fflush(stdout);
}

const char* errnoMsg(int Errno)
{
	::strerror_r(Errno, t_errnobuf, sizeof t_errnobuf);
	return t_errnobuf;
}

static Logger::LogLevel g_logLevel = initLogLevel();
static Logger::OutputFunc g_output = defaultOutput;
static Logger::FlushFunc g_flush = defaultFlush;

const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
  "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};

Logger::Impl::Impl(LogLevel level, int old_errno, const SourceFile& file, int line)
	:m_logLevel(level), m_stream(),m_time(Timestamp::now()), m_line(line), m_baseName(file)
{
	this->formatTime();
	m_stream << std::to_string(CurrentThread::tid());
	m_stream << LogLevelName[m_logLevel];
	if (old_errno != 0)
	{
		m_stream << errnoMsg(old_errno) << "(errno=" << old_errno << ")";
	}
}

void Logger::Impl::formatTime()
{
	
}

void Logger::Impl::finish()
{
	m_stream << " - " << std::string(m_baseName.m_data, m_baseName.m_size) << ':' << m_line << '\n';
}

Logger::Logger(SourceFile file, int line)
	:m_impl(LogLevel::INFO,0,file,line)
{

}

Logger::Logger(SourceFile file, int line, LogLevel level)
	: m_impl(level, 0, file, line)
{

}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
	: m_impl(level, 0, file, line)
{
	m_impl.m_stream << func << ' ';
}

Logger::Logger(SourceFile file, int line, bool toAbort)
	: m_impl(toAbort ? LogLevel::FATAL : LogLevel::ERROR, errno, file, line)
{

}

Logger::~Logger()
{
	m_impl.finish();
	const LogStream::BUFFER& buf(m_impl.m_stream.buffer());
	g_output(buf.data(), buf.lenght());//将缓冲区数据全部写入
	if (m_impl.m_logLevel == LogLevel::FATAL)//***严重出错
	{
		g_flush();//退出前刷新缓冲区，保证数据不丢失
		::abort();//退出程序
	}
}

inline Logger::LogLevel Logger::logLevel()
{
	return g_logLevel;
}

inline void Logger::setLogLevel(LogLevel level)
{
	g_logLevel = level;
}

