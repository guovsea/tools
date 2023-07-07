/**
 * @file AsyncLogging.cpp
 * @author {Guo} ({1926225569@qq.com})
 * @brief 创建一个
 * @version 0.1
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <assert.h>
#include "AsyncLogging.h"
#include "LogFile.h"
#include <stdio.h>
#include "unistd.h"


AsyncLogging::AsyncLogging(const std::string logFileName, int flushInterval = 2)
    :   m_flushInterval(flushInterval),
        m_running(false),
        m_logFileName(logFileName),
        m_thread(std::bind(&threadFunc(), this), "Logging"),
        m_mutex(),
        m_cond(m_mutex),
        m_currentBuffer(new Buffer),
        m_nextBuffer(new Buffer),
        m_buffers(),
        m_latch(1)
{
    assert(logFileName.size() > 1);
    m_currentBuffer->bzero();
    m_nextBuffer->bzero();
    m_buffers.reserve(16);
}
AsyncLogging::~AsyncLogging() {
    if (m_running)
        stop();
}
void AsyncLogging::append(const cahr* logLine, int lne)
{
    MutexLockGuard lock(m_mutex);                      // 加锁
    if (m_currentBuffer->avail() > len) {
        m_currentBuffer->append(logLine, len);
    } else { // 当前缓冲区满了
        m_buffers.push_back(m_currentBuffer);         // 将指向已满缓冲区的指针拷贝到 m_buffers 中
        m_currentBuffer.reset();                      // 将智能指针置空(而不是将缓冲区置空)
        if (m_nextBuffer) 
            m_currentBuffer = std::move(m_nextBuffer);
        else                                          // m_nextBuffer 指针也被置空
            m_currentBuffer.reset(new Buffer); 
        m_currentBuffer->append(logLine, len);
        m_cond.notify();
    }
}
// void AsyncLogging::append(const char* logLine, int len)
// {
//     MutexLockGuard lock(m_mutex);  // 加锁
//     if (m_currentBuffer->avail() > len) {
//         m_currentBuffer->append(logLine, len);
//     } else {  // 当前缓冲区满了
//         m_buffers.push_back(m_currentBuffer);  // 将指向已满缓冲区的指针拷贝到 m_buffers 中
//         m_currentBuffer.reset();  // 将智能指针置空(而不是将缓冲区置空)
//         // 在移动 m_nextBuffer 到 m_currentBuffer 之后立即进行日志写入操作
//         m_currentBuffer = std::move(m_nextBuffer);
//         m_currentBuffer->append(logLine, len);  // 写入日志
//         if (!m_nextBuffer) {
//             m_nextBuffer.reset(new Buffer);  // 申请新的缓冲区
//         }
//         m_cond.notify();
//     }
// }


void AsyncLogging::start()
{
    m_running = true;
    m_thread.start();
    m_latch.wait();
}
void AsyncLogging::stop()
{
    m_running = false;
    m_cond.notify();
    m_thread.join();
}

/**
 * @brief 日志工作线程
 * 
 */
void AsyncLogging::threadFunc()
{
    assert(m_running);
    m_latch.countDown();
    LogFile logFile(m_logFileName);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    newBuffer1->bzero();
    newBuffer2->bzero();
    BufferVector buffersToWrite;             // 保存已经写满的 buffer, 这些 buffer 将被写入日志中
    buffersToWrite.reserve(16);

    while (m_running) {
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        { // 上锁
        MutexLockGuard lock(m_mutex);
        if (m_buffers.empty()) {                      
            m_cond.waitForSeconds(m_flushInterval);
        }
        m_buffers.push_back(m_currentBuffer);
        m_currentBuffer.reset();
        m_currentBuffer = std::move(newBuffer1);
        buffersToWrite.swap(m_buffers);
        // 在临界区内将buffers中的所有内存块交换给buffersToWrite,然后再将 buffersToWrite 中的内容写入文件
        } // 解锁
        assert(!buffersToWrite.empty());     // bufferToWrite 不应该为空
        if (buffersToWrite.size() > 16) {
            // 如果待写入的 buffer 数量大于16, 则直接抛弃掉, 提高写入文件的速度
            buffersToWrite.erase(buffersToWrite.begin() + 16, buffersToWrite.end());
        }
        // 将buffersToWrite中所有的Buffer中的数据一次性写入 logFile中
        for (size_t i = 0; i < buffersToWrite.size(); ++i) {
            logFile.append(buffersToWrite[i]->data(), buffersToWrite[i]->length());
        }
        //只保留两个 buffer 并交给 newBuffer1 和 newBuffer2, 清空buffersToWrite中的所有 buffer 
        if (buffersToWrite.size() > 2) {
            buffersToWrite.resize(2);
        }
        if (!newBuffer1) {
            assert(!buffersToWrite.empty());  // bufferToWrite 不应该为空
            newBuffer1 = buffersToWrite.back();
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }
        if (!newBuffer2) {
            assert(!buffersToWrite.empty());  // bufferToWrite 不应该为空
            newBuffer2 = buffersToWrite.back();
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }
        buffersToWrite.clear();
        logFile.flush()
    }
    logFile.flush();
}