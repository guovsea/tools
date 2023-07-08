/**
 * @file Log.h
 * @author {Guo} ({1926225569@qq.com})
 * @brief 使用方法 LOG << 日志内容. 全局生成 AsyncLogging 的单例对象,用于将 LogStream写入头文件
 * @version 0.1
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef LOG_H
#define LOG_H
#include <string>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "LogStream.h"

//class AsyncLogging;

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

/**
 * @brief 生成一个 fileName = __FILE__, line = __LINE__的 Log 对象, 并调用 stream()方法
 * 
 */
#define LOG Log(__FILE__, __LINE__).stream()

#endif