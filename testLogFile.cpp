/**
 * @file testLogFile.cpp
 * @author {Guo} ({1926225569@qq.com})
 * @brief  g++ testLogFile.cpp LogFile.cpp LogFileUtil.cpp -std=c++11 -Wall && ./a.out
 * @version 0.1
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "LogFile.h"
#include "LogFileUtil.h"
#include "stdio.h"
#include <iostream>

int main(void)
{
    LogFile log("testLog.txt");
    const char *text = "apple";
    for (int i = 0; i < 100000; ++i) {
        char str[2];
        str[0] = *(text + i % 5);
        str[1] = '\n';
        log.append(str, 2);
    }
    return 0;
}
