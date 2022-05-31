#pragma once
#include "LogStream.h"
#include "../base/noncopyable.h"
#include <string.h>

class Logger :public noncopyable
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
    static void setLogLevel(LogLevel level);

private:
    /*
    * 成员封装类
    */
    class Impl
    {
    public:
        typedef Logger::LogLevel LogLevel;

        Impl(LogLevel level, int old_errno, const SourceFile& file, int line);

        void formatTime();
        void finish();


        LogStream m_stream;//流式数据
        LogLevel m_logLevel;//当前日志级别
        int m_line;//日志行数 由__line__得到
        SourceFile m_baseName;//日志所属文件名 ,由__file__与sourcefile_helper类得到
    };

    static LogLevel g_logLevel;
    Impl m_impl;

};