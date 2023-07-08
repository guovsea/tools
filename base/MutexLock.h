/**
 * @file MutexLock.h
 * @author {Guo} ({1926225569@qq.com})
 * @brief 封装互斥锁
 * @version 0.1
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef MUTEX_LOCK_H
#define MUTEX_LOCK_H
#include <pthread.h>
#include "NonCopyable.h"
#include <assert.h>

/**
 * @brief 封装互斥锁
 * 
 */
class MutexLock : NonCopyable
{
friend class Condition;   // 条件变量
public:
    MutexLock() { pthread_mutex_init(&m_mutex, NULL); }
    ~MutexLock() {
        int rest = pthread_mutex_lock(&m_mutex);
        assert(rest == 0); //Debug
        pthread_mutex_destroy(&m_mutex);
    }
    void lock() { pthread_mutex_lock(&m_mutex); }
    void unlock() { pthread_mutex_unlock(&m_mutex);}
    pthread_mutex_t *get() { return &m_mutex; }

private:
    pthread_mutex_t m_mutex;
};
/**
 * @brief 使用RAII机制管理 mutex,无需手动加锁解锁
 * @details 锁的生命周期等于 MUtexLockGuard 对象的生命周期
 */
class MutexLockGuard : NonCopyable {
public:
    explicit MutexLockGuard(MutexLock &mutex) : m_mutex(mutex) {
        m_mutex.lock();
    }
    ~MutexLockGuard() { m_mutex.unlock(); }

private:
    MutexLock &m_mutex;
};
#endif