/**
 * @file LogFileUtil.h
 * @author {Guo} ({1926225569@qq.com})
 * @brief  定义 LogFileAppender 和 LogBuffer
 * @version 0.1
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
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
    size_t write(const char *logline, size_t len);
    FILE *m_fp;
    char m_buffer[FILE_BUFFER_SIZE];
};

template <int SIZE>
class LogBuffer : NonCopyable
{
public:
    LogBuffer() : m_cur(nullptr) { m_cur = m_data;}
    ~LogBuffer() {}
    // 返回缓冲区剩余容量
    int avail() const { return static_cast<int>(end() - m_data);}
    // 容量不足返回-1
    int append(const char* buf, size_t len)
    {
        if (avail() > static_cast<int>(len)) {
            memcpy(m_cur, buf, len);
            m_cur += len;
            return len;
        }
        return -1;
    }
    const char* data() const { return m_data; }
    int length() const { return static_cast<int>(m_cur - m_data); }
    char* current() { return m_cur; }
    void add(size_t len) { m_cur += len; }

    void reset() { m_cur = m_data; }
    void bzero() { memset(m_data, 0, sizeof(m_data)); }

private:
    const char* end() const {return m_data + sizeof(m_data);}
    
    char m_data[SIZE];
    char* m_cur; // 读写指针
};
#endif