#pragma once
#include "LogStream.h"
#include "../base/noncopyable.h"
#include <string.h>

class Logger :public noncopyable
{
public:
    //��־�ȼ�
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
    * �ļ���
    * ���ڴ����ļ�����Ŀ¼
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

        const char* m_data;//source file�ĵ�ַ
        int m_size;//source file�ĳ���
    };

    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char* func);
    Logger(SourceFile file, int line, bool toAbort);
    ~Logger();

    //��ʽ��
    LogStream& stream()
    {
        return m_impl.m_stream;
    }
    //��ȡ��־�ȼ�
    static LogLevel logLevel();
    //������־�ȼ�
    static void setLogLevel(LogLevel level);

private:
    /*
    * ��Ա��װ��
    */
    class Impl
    {
    public:
        typedef Logger::LogLevel LogLevel;

        Impl(LogLevel level, int old_errno, const SourceFile& file, int line);

        void formatTime();
        void finish();


        LogStream m_stream;//��ʽ����
        LogLevel m_logLevel;//��ǰ��־����
        int m_line;//��־���� ��__line__�õ�
        SourceFile m_baseName;//��־�����ļ��� ,��__file__��sourcefile_helper��õ�
    };

    static LogLevel g_logLevel;
    Impl m_impl;

};