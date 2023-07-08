/**
 * @file Thread.h
 * @author {Guo} ({1926225569@qq.com})
 * @brief 封装线程
 * @details 封装之后的 Thread 对象符合 RAII 原则,Thread 对象的时候自动初始化线程参数(但此时
 * 线程并没有运行),Thread 对象析构时自动销毁线程。start 启动启动线程，join 阻塞调用者线程
 * @version 0.1
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <unistd.h>
#include <functional>
#include <string>
#include "NonCopyable.h"
#include "CountDownLatch.h"
#include <sys/prctl.h>
#include <sys/syscall.h>

class Thread : NonCopyable
{
public:
    typedef std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc&, const std::string& name = "");
    ~Thread();
    void start();
    int join();
    bool started() const { return m_started; }
    pid_t tid() const { return m_tid; }
    const std::string& name() const { return m_name; }


private:
    void setDefaultName();
    bool m_started;
    bool m_joined;
    pthread_t m_pthreadId;
    pid_t m_tid;
    ThreadFunc m_func;
    std::string m_name;
    CountDownLatch m_latch;
};
struct ThreadData 
{
    Thread::ThreadFunc m_func;
    std::string m_name;
    pid_t* m_tid_p;
    CountDownLatch* m_latch_p;
    ThreadData(const Thread::ThreadFunc& func, const std::string& name, pid_t* tid_p,
            CountDownLatch* latch_p) 
        : m_func(func), m_name(name), m_tid_p(tid_p), m_latch_p(latch_p) { }
    void run();

};

namespace CurrentThread {

extern thread_local int t_cachedTid;
extern thread_local char t_tidString[32];
extern thread_local int t_tidStringLength;
extern thread_local const char* t_threadName;


inline pid_t gettid() { return static_cast<pid_t>(::syscall(SYS_gettid)); }

inline void cacheTid() {
    if (t_cachedTid == 0) {
        t_cachedTid = gettid();
        t_tidStringLength =
        snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}

inline int tid() {
    if (t_cachedTid == 0) {
        cacheTid();
    }
    return t_cachedTid;
}


inline const char* tidString() { return t_tidString; }

inline int tidStringLength() { return t_tidStringLength; }

inline const char* name() { return t_threadName; }

} // namespace CurrentThread


#endif