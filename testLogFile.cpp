#include "LogFile.h"
#include "stdio.h"
#include <iostream>

int main(void)
{
    LogFile log("testLog.txt");
    const char *text = "apple";
    for (int i = 0; i < 100000; ++i) {
        char str[2];
        strcat(str, text + i % 5);
        log.append(str, 2);
    }
    return 0;
}
