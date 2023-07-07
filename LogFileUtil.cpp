#include <stdio.h>
#include "LogFileUtil.h"

/**
 * @brief 打开指定文件并创建 LogFileAppender 对象, 
 *  同时自定义文件缓冲区大小为 FILE_BUFFER_SIZE(默认64 * 1024)
 * 
 * @param fileName 文件名
 */
LogFileAppender::LogFileAppender(std::string fileName) 
    : m_fp(fopen(fileName.c_str(), "ae"))
{
    setbuffer(m_fp, m_buffer, sizeof(m_buffer));
}

LogFileAppender::~LogFileAppender()  { fclose(m_fp); }

void LogFileAppender::append(const char *logLine, const size_t len)
{
    size_t n = this->write(logLine, len);
    size_t remain = len - n;
    while (remain > 0) {
        // 反复尝试写, 保证能够全部成功写入
        size_t x = this->write(logLine + n, remain);
        if (x == 0) {
            int err = ferror(m_fp);
            if (err) 
                fprintf(stderr, "LogFileAppender::append() faild! ");
            break;
        }
        n += x;
        remain = len - n;
    }
    
}

void LogFileAppender::flush() { fflush(m_fp);}
/**
 * @brief 使用无锁的 fwrite, 速度更快, 但是得保证单线程写
 * 
 * @param logline 写入的 log 行
 * @param len 需要写入的字节数
 * @return size_t 成功写入的字节数
 */
size_t LogFileAppender::write(const char* logline, size_t len) {
    return fwrite_unlocked(logline, 1, len, m_fp);
}