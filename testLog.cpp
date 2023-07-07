/**
*/
#include "Log.h"
#include "Thread.h"
#include <string>
#include <unistd.h>
#include <vector>
#include <memory>
#include <iostream>
using namespace std;

void type_test()
{
    // 13 lines
    cout << "--------------type test---------------" << endl;
    LOG << 0;
    LOG << 1234567890123;
    LOG << 1.0f;
    LOG << 3.1415926;
    LOG << (short) 1;
    LOG << (long long) 1;
    LOG << (unsigned int) 1;
    LOG << (unsigned long) 1;
    LOG << (long double) 1.6555556;
    LOG << (unsigned long long) 1;
    LOG << 'c';
    LOG << "abcdefg";
    LOG << string("This is a string");
}

void stream_test()
{
    // 1 line
    cout << "-------------stream test--------------" << endl;
    LOG << "fddsa" << 'c' << 0 << 3.666 << string("This is a string");
}

void threadFunc()
{
    for (int i = 0; i < 100000; ++i)
    {
        LOG << i;
    }
}

void test_single_thread()
{
    // 100000 lines
    cout << "----------test single thread----------" << endl;
    for (int i = 0; i < 100000; ++i)
    {
        LOG << i;
    }
}

void test_multi_threads(int threadNum = 4)
{
    // threadNum * 100000 lines
    cout << "----------test multi thread-----------" << endl;
    vector<shared_ptr<Thread>> vsp;
    for (int i = 0; i < threadNum; ++i)
    {
        shared_ptr<Thread> tmp(new Thread(threadFunc, "testFunc"));
        vsp.push_back(tmp);
    }
    for (int i = 0; i < threadNum; ++i)
    {
        vsp[i]->start();
    }
    sleep(3);
}


int main()
{
    // 共500014行
    type_test();

    stream_test();

    test_single_thread();

    test_multi_threads();
    return 0;
}