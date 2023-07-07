/**
 * @file LogFile.h
 * @author {Guo} ({1926225569@qq.com})
 * @brief LogFile 用于封装日志文件对象,包括对日志的追加写和强制刷入磁盘
 * @version 0.1
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef LOG_FILE_H
#define LOG_FILE_H
#include <memory>
#include <string>
#include "NonCopyable.h"
#include "MutexLock.h"
#include "LogFileUtil.h"

class LogFile : NonCopyable
{
public:
    LogFile(const std::string& logFileName, int flushEveryN = 1024);
    ~LogFile();

    void append(const char* logLine, int len);
    /**
     * @brief 写入文件时也存在一个缓冲区, 使用 flush 强制将缓冲区的内容刷入文件
     * 
     */
    void flush();

private:
    void append_unlocked(const char* logLine, int len);

    const std::string m_logFileName;
    const int m_flushEveryN;
    int m_count;
    std::unique_ptr<MutexLock> m_mutex;
    std::unique_ptr<LogFileAppender> m_file;

};

#endif