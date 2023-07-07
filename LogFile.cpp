#include "LogFile.h"

LogFile::LogFile(const std::string& baseName, int flushEveryN)
    : m_baseName(baseName), m_flushEveryN(flushEveryN),
        m_count(0), m_mutex(new MutexLock)
{
    m_file.reset(new LogFileAppender(baseName));
}

LogFile::~LogFile() {}

void LogFile::append(const char* logLine, int len)
{
    MutexLockGuard lock(*m_mutex);
    append_unlocked(logLine, len);
}
void LogFile::flush()
{
    MutexLockGuard lock(*m_mutex);
    m_file->flush();
}

void LogFile::append_unlocked(const char* logLine, int len)
{
    m_file->append(logLine, len);
    ++m_count;
    if (m_count >= m_flushEveryN) {
        m_count = 0;
        m_file->flush();
    }
}