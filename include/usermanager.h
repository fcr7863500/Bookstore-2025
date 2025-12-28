#pragma once
#ifndef BOOKSTORE_2025_USERMANAGER_H
#define BOOKSTORE_2025_USERMANAGER_H
#include"file storage.h"
#include"makearray.h"
#include"tool.h"
#include<vector>
#include<string>
#include<stack>

struct UserInfo
{
    MakeArray password;
    MakeArray username;
    int privilege;
    bool is_logged;

    UserInfo();
    UserInfo(const std::string& pw,const std::string& us,int priv);
    bool operator<(const UserInfo& other) const;
    bool operator==(const UserInfo& other) const;
    bool operator>(const UserInfo& other) const;
    bool operator!=(const UserInfo& other) const;
    bool operator<=(const UserInfo& other) const;
    bool operator>=(const UserInfo& other) const;
};
class UserManager
{
private:
    LinkedBlock<MakeArray,UserInfo> user_storage;
    std::stack<MakeArray> login_stack;
    MakeArray cur_user;
    int cur_privilege;
    void Initial_Root_user();
public:
    UserManager() : user_storage("users.dat"),cur_privilege(0)
    {
        // std::cerr << "construct user manager" << std::endl;
        Initial_Root_user();
        // std::cerr << "constructed user manager" << std::endl;
    }
    bool su(const std::string& id,const std::string& password = "");
    bool logout();
    bool registerUser(const std::string& id,const std::string& password,const std::string& username);
    bool passwd(const std::string& id,const std::string& cur_pw,const std::string& new_pw);
    bool useradd(const std::string& id,const std::string& pw,int priv,const std::string& username);
    bool deleteUser(const std::string& id);
    int getUserPrivilege() const;
    std::string getCurUser() const;
    bool isLoggedIn(const std::string& id);
};
#endif //BOOKSTORE_2025_USERMANAGER_H