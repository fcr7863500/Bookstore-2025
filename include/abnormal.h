#pragma once
#ifndef BOOKSTORE_2025_ABNORMAL_H
#define BOOKSTORE_2025_ABNORMAL_H
#include <exception>
#include "CustomString.hpp"


class BookstoreException : public std::exception
{
protected:
    CustomString err_msg_;
public:
    BookstoreException(const CustomString& msg) : err_msg_(msg) {}
    virtual const char* what() const noexcept override {
        return err_msg_.c_str();
    }
    virtual ~BookstoreException() = default;
};

class PrivilegeException : public BookstoreException {
public:
    PrivilegeException() : BookstoreException("Insufficient privilege") {}
};

// 参数异常
class ParameterException : public BookstoreException {
public:
    ParameterException() : BookstoreException("Invalid parameter") {}
};

// 数据异常（如账户/图书不存在）
class DataException : public BookstoreException {
public:
    DataException() : BookstoreException("Data not found or duplicated") {}
};

// 状态异常（如未登录/未选书）
class StateException : public BookstoreException {
public:
    StateException() : BookstoreException("Invalid system state") {}
};
#endif //BOOKSTORE_2025_ABNORMAL_H