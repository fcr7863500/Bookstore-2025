#pragma once
#ifndef BOOKSTORE_2025_MAKEARRAY_H
#define BOOKSTORE_2025_MAKEARRAY_H
#include<iostream>
#include<string>
#include<cstring>
#include<cctype>
#include<algorithm>
//一个自定义的固定长度的特殊数组类，用于将所有的字符数组和字符串（长度不固定）统一，便于文件读写
class MakeArray
{
private:
    static const size_t MAX_SIZE = 65;//最大长度
    char array[MAX_SIZE];
    //初始化
    void clear()
    {
        memset(array, '\0', sizeof(array));
    }
public:
    //初始化构造均为‘\0’
    MakeArray()
    {
        clear();
    }
    //将普通的字符型数组转化为MakeArray类型
    MakeArray(const char* str)
    {
        clear();
        if (str != nullptr)
        {
            strncpy(array,str,MAX_SIZE - 1);
        }
    }
    //将字符串类型的数组转化为MakeArray类型
    MakeArray(const std::string& str)
    {
        clear();
        strncpy(array,str.c_str(),MAX_SIZE - 1);
    }
    //拷贝构造
    MakeArray(const MakeArray& other)
    {
        memcpy(array,other.array,MAX_SIZE);
    }
    //赋值运算符重载
    MakeArray& operator=(const MakeArray& other)
    {
        if (this != &other)
        {
            memcpy(array,other.array,MAX_SIZE);
        }
        return *this;
    }
    MakeArray& operator=(const char* str)
    {
        clear();
        if (str != nullptr)
        {
            strncpy(array,str,MAX_SIZE - 1);
        }
        return *this;
    }
    MakeArray& operator=(const std::string& str)
    {
        clear();
        strncpy(array,str.c_str(),MAX_SIZE - 1);
        return *this;
    }
    operator const char*() const
    {
        return array;
    }
    std::string toString() const
    {
        return std::string(array,strnlen(array,MAX_SIZE));
    }
    bool isEmpty() const
    {
        return strnlen(array,MAX_SIZE) == 0;
    }
    //比较运算符重载
    //和自身类型的比较
    bool operator<(const MakeArray& other) const
    {
        return strcmp(array,other.array) < 0;
    }
    bool operator>(const MakeArray& other) const
    {
        return strcmp(array,other.array) > 0;
    }
    bool operator==(const MakeArray& other) const
    {
        return strcmp(array,other.array) == 0;
    }
    bool operator!=(const MakeArray& other) const
    {
        return !(*this == other);
    }
    bool operator<=(const MakeArray& other) const
    {
        return !(*this > other);
    }
    bool operator>=(const MakeArray& other) const
    {
        return !(*this < other);
    }
    //和字符数组比较
    bool operator<(const char* str) const
    {
        return std::strcmp(array,str) < 0;
    }
    bool operator>(const char* str) const
    {
        return std::strcmp(array,str) > 0;
    }
    bool operator==(const char* str) const
    {
        return std::strcmp(array,str) == 0;
    }
    bool operator!=(const char* str) const
    {
        return !(*this == str);
    }
    bool operator<=(const char* str) const
    {
        return !(*this > str);
    }
    bool operator>=(const char* str) const
    {
        return !(*this < str);
    }
    //与字符串进行比较
    bool operator<(const std::string& str) const
    {
        return std::strcmp(array,str.c_str()) < 0;
    }
    bool operator>(const std::string& str) const
    {
        return std::strcmp(array,str.c_str()) > 0;
    }
    bool operator==(const std::string& str) const
    {
        return std::strcmp(array,str.c_str()) == 0;
    }
    bool operator!=(const std::string& str) const
    {
        return !(*this == str);
    }
    bool operator<=(const std::string& str) const
    {
        return !(*this > str);
    }
    bool operator>=(const std::string& str) const
    {
        return !(*this < str);
    }
    static size_t getMaxSize()
    {
        return MAX_SIZE;
    }
};
#endif //BOOKSTORE_2025_MAKEARRAY_H