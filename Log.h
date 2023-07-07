#ifndef LOG_H
#define LOG_H
#include <string>
#include <pthread.h>
#include "LogStream.h"

class Log
{
public:
    Log(const char *fileName, int line);
    ~Log();
    LogStream& stream() { return m_impl.m_stream; }
    static void setLoFileName(std::string logFileName) {m_logFileName = logFileName; }
    static std::string getLogFileName() { return m_logFileName; }

private:
    struct Impl
    {
        Impl(const char *fileName, int line);
        void formatTime();
        LogStream m_stream;
        int m_line;
        std::string m_fileName;
    };
    Impl m_impl;
    static std::string m_logFileName;
};

#define LOG Log(__FILE__, __LINE__).stream()

#endif