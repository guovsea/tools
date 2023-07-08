#ifndef COUNT_DOWN_LATCH_H
#define COUNT_DOWN_LATCH_H

#include "NonCopyable.h"
#include "MutexLock.h"
#include "Condition.h"
/**
 * @brief 等待某组事件完成
 * 
 * @param count 等待事件的数量
 */
class CountDownLatch : NonCopyable
{
public:
    explicit CountDownLatch(int count) : m_mutex(), m_cond(m_mutex), m_count(count){ }
    void wait() {
        MutexLockGuard lock(m_mutex);
        while (m_count > 0) m_cond.wait();
    }
    void countDown() {
        MutexLockGuard lock(m_mutex);
        --m_count;
        if (m_count == 0)
            m_cond.notifyAll();

    }
private:
    mutable MutexLock m_mutex;
    Condition m_cond;
    int m_count;
};

#endif 