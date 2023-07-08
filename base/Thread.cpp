#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <memory>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include "Thread.h"

/**
 * @brief 线程局部变量
 * @details 每个线程拥有其自己的变量，在不同的线程中具有不同的值
 */
namespace CurrentThread {

thread_local int t_cachedTid = 0;
thread_local char t_tidString[32];
thread_local int t_tidStringLength = 6;
thread_local const char* t_threadName = "default";
}

void ThreadData::run()
{
    *m_tid_p = CurrentThread::tid();
    m_tid_p = NULL;
    m_latch_p->countDown(); // ThreadData 初始化完成并开始运行后,唤醒主线程
    m_latch_p = NULL;

    CurrentThread::t_threadName = m_name.empty() ? "Thread" : m_name.c_str();
    prctl(PR_SET_NAME, CurrentThread::t_threadName);

    m_func();
    // 传入的函数已经运行结束
    CurrentThread::t_threadName = "finished";
}

Thread::Thread(const ThreadFunc& func, const std::string& name)
    : m_started(false),m_joined(false), m_pthreadId(0),
    m_tid(0), m_func(func), m_name(name), m_latch(1) 
{
    setDefaultName();
}

Thread::~Thread()
{
    if (m_started && !m_joined)
        pthread_detach(m_pthreadId);
}

// 线程运行函数
void* work(void* obj) {
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->run();
    delete data;
    return NULL;
}

void Thread::start()
{
    assert(!m_started);
    m_started = true;
    ThreadData* data = new ThreadData(m_func, m_name , &m_tid, &m_latch);
    if (pthread_create(&m_pthreadId, NULL, &work, data)) {
        m_started = false;
        delete data;
        //perror
    } else {
        m_latch.wait(); // 阻塞主线程
        assert(m_tid > 0);
    }
}

int Thread::join()
{
    assert(m_started);
    assert(!m_joined);
    m_joined = true;
    return pthread_join(m_pthreadId, NULL);
}

void Thread::setDefaultName()
{
    if (m_name.empty())
        m_name = "Thread";
}