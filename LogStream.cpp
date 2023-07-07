
#include <string>
#include <stdio.h>
#include <iostream>
#include "LogStream.h"
#include "LogFileUtil.h"

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;

/**
 * @brief 将 value 转成字符串,放入 buf 中
 * 
 * @param buf 目标缓冲区
 * @return size_t 转换后的字符串的长度
 */
template <typename T>
size_t convert(char buf[], T value)
{
    char *p = buf;
    do {
        int digit = static_cast<int>(value % 10);
        value /= 10;
        *p++ = zero[digit];
    } while (value != 0);
    if (value < 0) {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);
    return p - buf;
}

template class LogBuffer<kSmallBuffer>;
template class LogBuffer<kLargeBuffer>;

/**
 * @brief 如果 buffer 剩余空间小于 32个字节, 待写入的整数会被直接丢弃
 * 
 * @tparam T 整型类型
 * @param v 需要写入的整数
 */
template <typename T>
void LogStream::writeInteger(T v) {
    if (m_buffer.avail() >= kMaxNSize) {
        size_t len = convert(m_buffer.current(), v);
        m_buffer.add(len);
    }
}

LogStream& LogStream::operator<<(short v)
{
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v)
{
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v)
{
    writeInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v)
{
    writeInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long v)
{
    writeInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v)
{
    writeInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v)
{
    writeInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
    writeInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(float v)
{
    *this << static_cast<double>(v);
    return *this;
}

LogStream& LogStream::operator<<(double v)
{
    if (m_buffer.avail() >= kMaxNSize) {
        int len = snprintf(m_buffer.current(), kMaxNSize, "%.12g", v);
        m_buffer.add(len);
    }
    return *this;
}

LogStream& LogStream::operator<<(char v)
{
    m_buffer.append(&v, 1);
    return *this;
}

LogStream& LogStream::operator<<(const char* str)
{
    if (str != nullptr)
        m_buffer.append(str, strlen(str));
    else
        m_buffer.append("(null)", 6);
    return *this;
    
}

LogStream& LogStream::operator<<(const unsigned char* str)
{
    return operator<<(reinterpret_cast<const char*>(str));
}

LogStream& LogStream::operator<<(const std::string& str)
{
    m_buffer.append(str.c_str(), str.size());
    return *this;
}

// int main (void) {
//     // char buf[100];
//     // int num = convert(buf, 31343);
//     // for (int i = 0; i < num; ++i) {
//     //     printf("%c", buf[i]);
//     // }
//     LogStream log;
//     log.resetBuffer();
//     log << "this is a char";
//     for (int i = 0; i < log.buffer().length(); ++i) {
//         std::cout << log.buffer().data()[i];
//     }
// }