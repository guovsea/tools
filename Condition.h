/**
 * @file Condition.h
 * @author {Guo} ({1926225569@qq.com})
 * @brief 封装条件变量
 * @version 0.1
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CONDITION_H
#define CONDITION_H
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
#include "MutexLock.h"
#include "NonCopyable.h"
#include <iostream>

class Condition : NonCopyable
{
public:
    explicit Condition(MutexLock & mutex) :m_mutex(mutex) {
        pthread_cond_init(&m_cond, NULL);
    }
    ~Condition() { pthread_cond_destroy(&m_cond); }
    void wait() { pthread_cond_wait(&m_cond, m_mutex.get()); }
    void notify() { pthread_cond_signal(&m_cond); }
    void notifyAll() {pthread_cond_broadcast(&m_cond); }
    /**
     * @brief 阻塞一段时间后检查条件变量是否满足
     * 
     * @param seconds 阻塞时间
     * @return true 
     * @return false 
     */
    bool waitForSeconds(int seconds) {
        struct timespec  abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        assert(m_mutex.get() != NULL);  // Debug
        return ETIMEDOUT == pthread_cond_timedwait(&m_cond, m_mutex.get(), &abstime);
    }

private:
    MutexLock &m_mutex;
    pthread_cond_t m_cond;
};
#endif