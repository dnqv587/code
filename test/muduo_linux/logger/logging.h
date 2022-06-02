#pragma once
#include "LogStream.h"
#include "../base/noncopyable.h"
#include "../time/Timestamp.h"
#include <string.h>

class Logger :private noncopyable
{
public:
    //日志等级
	enum LogLevel
	{
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
	};

    /*
    * 文件名
    * 用于处理文件名和目录
    */
    class SourceFile
    {
    public:
        template<int N>
        SourceFile(const char(&arr)[N])
            :m_data(arr),m_size(N)
        {
            const char* slash= ::strrchr(m_data, '/');
            if (slash)
            {
                m_data = slash + 1;
                m_size -= m_data - arr;
            }
        }

        SourceFile(const char* FileName)
            :m_data(FileName)
        {
            const char* slash = ::strrchr(m_data, '/');
            if (slash)
            {
                m_data = slash + 1;
                m_size = strlen(m_data);
            }
        }

        const char* m_data;//source file的地址
        int m_size;//source file的长度
    };

    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char* func);
    Logger(SourceFile file, int line, bool toAbort);
    ~Logger();

    //流式化
    LogStream& stream()
    {
        return m_impl.m_stream;
    }
    //获取日志等级
    static LogLevel logLevel();
    //设置日志等级
    static void setLogLevel(Logger::LogLevel level);

    typedef void (*OutputFunc)(const char* msg, int len);
	typedef void (*FlushFunc)();
    /*
    * 使用output和flush回调主要是用来解耦合，通过设置回调函数来使日志可以同步或异步的方式输出到指定的方向
    * 默认输出到stdout
    */
    //设置输出回调
	static void setOutput(OutputFunc out);
    //设置刷新回调
	static void setFlush(FlushFunc flush);

private:
    /*
    * 成员封装类
    */
    class Impl
    {
    public:
        typedef Logger::LogLevel LogLevel;
        //level:日志等级，old_errno：错误号，file：文件名，line：行号
        Impl(LogLevel level, int old_errno, const SourceFile& file, int line);

        void formatTime();
        void finish();

        Timestamp m_time;//时间
        LogStream m_stream;//流式数据
        LogLevel m_logLevel;//当前日志级别
        int m_line;//日志行数 由__line__得到
        SourceFile m_baseName;//日志所属文件名 ,由__file__与sourcefile_helper类得到
    };


    //static LogLevel g_logLevel;//日志等级
    //static OutputFunc g_output;//日志输出回调
    //static FlushFunc g_flush;//刷新缓冲区回调
    Impl m_impl;

};


#define LOG_TRACE if (Logger::logLevel() <=Logger::TRACE) \
  Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (Logger::logLevel() <= Logger::DEBUG) \
  Logger(__FILE__, __LINE__, Logger::DEBUG, __func__).stream()
#define LOG_INFO if (Logger::logLevel() <= Logger::INFO) \
  Logger(__FILE__, __LINE__).stream()
#define LOG_WARN Logger(__FILE__, __LINE__, Logger::WARN).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::ERROR).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__, Logger::FATAL).stream()
#define LOG_SYSERR Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL Logger(__FILE__, __LINE__, true).stream()

//检查是否为空
#define CHECK_NOTNULL(val) checkNotNULL(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

template <typename T>
T* checkNotNULL(Logger::SourceFile file, int line, const char* names, T* ptr)
{
    if (!ptr)
    {
        Logger(file, line, Logger::FATAL).stream() << names;
    }
    return ptr;
}
