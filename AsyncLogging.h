/**
 * @file AsyncLogging.h
 * @author {Guo} ({1926225569@qq.com})
 * @brief AsyncLogging对象创建一个日志线程, 不断将产生的日志写入缓冲区中, 间隔一定时间将缓冲区中的日志刷入文件
 * @details 使用了双缓冲区机制. AsyncLogging 对象中维护着两个空 buffer :currentBuffer 和 nextBuffer 
 * 以及一个被写满了的 Buffer 队列. currentBuffer 被写满时, 将 currentBuffer 放到已写满队列, 然后将 nextBuffer
 * 移动到 currentBuffer, 继续将日志写入缓冲. 写完之后才再次申请一块 buffer 并用 nextBuffer 指向它.
 * @version 0.1
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef ASYNCLOGGING_H
#define ASYNCLOGGING_H
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include "NonCopyable.h"
#include "MutexLock.h"
#include "CountDownLatch.h"
#include "Thread.h"
#include "LogStream.h"

class AsyncLogging : NonCopyable
{
public:
    AsyncLogging(const std::string logFileName, int flushInterval = 2);
    ~AsyncLogging();
    void append(const char* logLine, int lne);
    void start();
    void stop();
private:
    void threadFunc();
    //typedef LogBuffer<kLargeBuffer> Buffer;
    typedef LogBuffer<kLargeBuffer> Buffer;
    typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
    typedef std::shared_ptr<Buffer> BufferPtr;
    const int m_flushInterval;
    bool m_running;
    std::string m_logFileName;
    Thread m_thread;
    MutexLock m_mutex;
    Condition m_cond;
    BufferPtr m_currentBuffer;
    BufferPtr m_nextBuffer;
    BufferVector m_buffers;
    CountDownLatch m_latch;
};

#endif