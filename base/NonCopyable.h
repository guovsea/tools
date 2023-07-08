/**675
 * @file NonCopyable.h
 * @author {Guo} ({1926225569@qq.com})
 * @brief 阻止拷贝
 * @version 0.1
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef NON_COPYABLE_H
#define NON_COPYABLE_H
/**
 * @brief 阻止拷贝
 * 
 */
class NonCopyable
{
public:
    NonCopyable() {}
    ~NonCopyable() {}
    NonCopyable(const NonCopyable&) = delete;
    const NonCopyable& operator=(const NonCopyable&) = delete;
};
#endif