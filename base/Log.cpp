#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "Log.h"
#include "Thread.h"
#include "AsyncLogging.h"
#include <iostream>


//Log::setLoFileName("./WebServer.log");

/**
 * @brief 单例模式, 确保只创建一个 AsyncLogging 对象
 * 
 */
static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
static AsyncLogging* AsyncLogger_;

std::string Log::m_logFileName = "./WebServer.log";

void once_init()
{
    AsyncLogger_ = new AsyncLogging(Log::getLogFileName());
    AsyncLogger_->start();
}

void output(const char* msg, int len)
{
    pthread_once(&once_control_, once_init);
    AsyncLogger_->append(msg, len);
}

void Log::Impl::formatTime()
{
    struct timeval tv;
    time_t time;
    char str_time[26] = {0};
    gettimeofday(&tv, NULL);
    time = tv.tv_sec;
    struct tm* p_time = localtime(&time);
    strftime(str_time, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
    m_stream << str_time;
}
Log::Log(const char *fileName, int line) : m_impl(fileName, line) 
{
    //std::cout << "Log::Log" << std::endl; // test
}

Log::Impl::Impl(const char *fileName, int line)
    : m_stream(),m_line(line), m_fileName(fileName)
{
    formatTime();
}
/// @brief  Log 对象每次析构的时候都会通过 output 函数, 调用 AsyncLogging 的 append 方法
///         将Log 对象的 LogStream 的缓冲区复制到 AsyncLogging 的缓冲区中
Log::~Log()
{
    m_impl.m_stream << " -- " << m_impl.m_fileName<< ':' << m_impl.m_line << '\n';
    const LogStream::Buffer& buf(stream().buffer());
    output(buf.data(), buf.length());
}

