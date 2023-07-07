#include <assert.h>
#include <time.h>
#include "Log.h"
#include "Thread.h"
#include "AsyncLogging.h"

std::string Log::m_logFileName = "./WebServer.log";
//Log::setLoFileName("./WebServer.log");

/**
 * @brief 单例模式, 确保只创建一个 AsyncLogging 对象
 * 
 */
static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
static AsyncLogging *AsyncLogger_;

void once_init()
{
    AsyncLogger_ = new AsyncLogging(Log::getLogFileName());
    AsyncLogger_->start();
}
void output(const cahr* msg, int len)
{
    pthread_once(&once_control_, once_init);
    AsyncLogger_->append(msg, len);
}

Log::Impl::formatTime()
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
Log::Log(const char *fileName, int line) : m_impl(fileName, line) { }
~Log()
{

}
Log::Impl::Impl(const char *fileName, int line)
    : m_stream(),m_line(line), m_logFileName(fileName)
{
    formatTime();
}
Log::~Log()
{
    impl.m_stream << " -- " << impl.m_logFilename << ':' << impl.m_line << '\n';
    const LogBuffer::Buffer& buf(stream().buffer());
    output(buf.data(), buf.length());
}

