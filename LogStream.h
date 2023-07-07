/**
 * @file LogStream.h
 * @author {Guo} ({1926225569@qq.com})
 * @brief LogStream 允许将各种内置类型的数据以流式的方式写入 LogBuffer 中
 * @version 0.1
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef LOG_STREAM_H
#define LOG_STREAM_H
#include <assert.h>
#include <string>
#include <string.h>
#include "NonCopyable.h"
#include "LogFileUtil.h"

// 4028
constexpr int kSmallBuffer = 4028;
// 4MB
constexpr int kLargeBuffer = 4028 * 1024;
// 能够完整写入整数的最小剩余 buffer 空间
constexpr const int kMaxNumericSize = 32;

class LogStream : NonCopyable
{
public:
    typedef LogBuffer<kSmallBuffer> Buffer;
    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int );
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);
    LogStream& operator<<(float);
    LogStream& operator<<(double);
    LogStream& operator<<(char);
    LogStream& operator<<(const char*);
    LogStream& operator<<(const unsigned char* );
    LogStream& operator<<(const std::string&);

    void append(const char* data, int len) { m_buffer.append(data, len); }
    const Buffer& buffer() const { return m_buffer; };
    void resetBuffer() { m_buffer.reset(); }

private:
    //void staticCheck();

    template<typename T>
    void writeInteger(T);

    Buffer m_buffer;
    // kMaxNumericSize
    static const int kMaxNSize = kMaxNumericSize;

};

#endif