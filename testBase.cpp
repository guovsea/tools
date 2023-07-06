/**
 * @file testBase.cpp
 * @author {Guo} ({1926225569@qq.com})
 * @brief 测试 base 模块 g++ Thread.cpp testBase.cpp -std=c++11 -Wall -pthread -g
 * @version 0.1
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <time.h>
#include <vector>
#include <queue>
#include <memory>
#include "MutexLock.h"
#include "Condition.h"
#include "Thread.h"
using std::cout;
using std::endl;

MutexLock mutex;
int n = 0;
const int MaxBufferSize = 5;
std::queue<int> buffer;
MutexLock pc_mutex;
Condition full(pc_mutex);
Condition empty(pc_mutex);

void run()
{
    
    for (int i = 0; i < 100000000; ++i) {
        MutexLockGuard guard(mutex);
        ++n;
    }
}

void producer() {
    for (int i = 1; i <= 10; ++i) {
        MutexLockGuard guard(pc_mutex);
        while (buffer.size() >= MaxBufferSize) {
            full.wait();
        }
        buffer.push(i);
        std::cout << "Produced: " << i << std::endl;
        empty.notify();
    }
}

void consumer() {
    for (int i = 1; i <= 10; ++i) {
        MutexLockGuard guard(pc_mutex);
        while (buffer.empty()) {
            empty.wait();
        }
        int data = buffer.front();
        buffer.pop();
        std::cout << "Consumed: " << data << std::endl;
        full.notify();
    }
}

void testThread()
{
    cout << "============ test Thread ============" << endl;
    Thread thrd1(
        [&thrd1](){
            cout << "   Thread running" << endl;
            cout << "   " << "Thread name : " << CurrentThread::name() << endl;
            cout << "   Thread tid = " << thrd1.tid() << " name = " << thrd1.name() << endl;
            cout << "   " << "Thread name : " << CurrentThread::name() << endl;
            cout << "   Thread running" << endl;
            });
    cout << "Thread tid = " << thrd1.tid() << " name = " << thrd1.name() << endl;
    thrd1.start();
    thrd1.join();
    cout << "Thread name : " << CurrentThread::name() << endl;

}
void testMutexLock()
{
    cout << "========== test MutexLock ==========" << endl;
    int thread_n = 10;
    std::vector<std::unique_ptr<Thread>> threads;

    for (int i = 0; i < thread_n; ++i) {
        std::unique_ptr<Thread> pthrd(new Thread(run));
        threads.push_back(std::move(pthrd));
    }
    for (int i = 0; i < thread_n; i++) {
        threads[i]->start();
    }
    for (int i = 0; i < thread_n; i++) {
        threads[i]->join();
    }
    cout << "result n = " << n << endl;
    cout << " right n = 1000000000" << endl;
}

void testCondition()
{
    cout << "============ test Condition ============" << endl;
    Thread producerThrad(producer);
    Thread consumerThread(consumer);
    producerThrad.start();
    consumerThread.start();
    producerThrad.join();
    consumerThread.join();
}
int main(void) {
    testThread();
    testMutexLock();
    testCondition();
    
    return 0;
}