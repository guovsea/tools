#ifndef LOG_FILE_UTIL_H
#define LOG_FILE_UTIL_H
#include <string>
#include "NonCopyable.h"
#define FILE_BUFFER_SIZE  64 * 1024
class LogFileAppender : NonCopyable
{
public:
    explicit LogFileAppender(std::string fileName);
    ~LogFileAppender();
    void append(const char *logLine, const size_t len);
    void flush();

private:
    size_t write(const char *logLine, size_t len);
    FILE *m_fp;
    char m_buffer[FILE_BUFFER_SIZE];
};
#endif