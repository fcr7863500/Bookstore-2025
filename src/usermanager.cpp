#include"../include/usermanager.h"
//构造userinfo
UserInfo::UserInfo() : privilege(0),is_logged(false){}

UserInfo::UserInfo(const std::string& pw,const std::string& us,int priv):
password(pw),username(us),privilege(priv){}

bool UserInfo::operator<(const UserInfo& other) const
{
    if (password != other.password) return (password < other.password);
    if (username != other.username) return (username < other.username);
    if (privilege != other.privilege) return (privilege < other.privilege);
    return is_logged < other.is_logged;
}

bool UserInfo::operator==(const UserInfo& other) const
{
    return (password == other.password && username == other.username && privilege == other.privilege && is_logged == other.is_logged);
}

bool UserInfo::operator>(const UserInfo& other) const
{
    if (password != other.password) return (password > other.password);
    if (username != other.username) return (username > other.username);
    if (privilege != other.privilege) return (privilege > other.privilege);
    return is_logged > other.is_logged;
}

bool UserInfo::operator!=(const UserInfo& other) const
{
    return !(*this == other);
}

bool UserInfo::operator<=(const UserInfo& other) const
{
    return !(*this > other);
}

bool UserInfo::operator>=(const UserInfo& other) const
{
    return !(*this < other);
}

 void UserManager::Initial_Root_user()
{
    MakeArray root_id("root");
    std::vector<UserInfo> root_info = user_storage.find(root_id);
    if (root_info.empty())
    {
        UserInfo root{"sjtu","root",7};
        user_storage.insert(root_id,root);
        std::cerr << user_storage.find(root_id).size() << std::endl;
    }
}
//有个小问题，如果该用户已经进入登录栈，重复登录效果是什么样
bool UserManager::su(const std::string& id,const std::string& password)
{
    auto vec = user_storage.getAll();
    for (auto i : vec)
    {
        std::cerr << "user: ";
        std::cerr << i.username.toString() << std::endl;
    }
    if (id.empty())
    {
        std::cerr << "empty id" << std::endl;
        Tool::printInvalid();
        return false;
    }
    if (!Tool::isValidUserID(id))
    {
        std::cerr << "invalid id" << std::endl;
        Tool::printInvalid();
        return false;
    }
    MakeArray uid(id);
    std::vector<UserInfo> infos = user_storage.find(uid);
    if (infos.empty())
    {
        std::cerr << "empty info" << std::endl;
        Tool::printInvalid();
        return false;
    }
    UserInfo& user = infos[0];
    if (cur_privilege > user.privilege)
    {
        if (!password.empty())
        {
            if (!Tool::isValidPassword(password) || user.password != MakeArray(password))
            {
                Tool::printInvalid();
                return false;
            }
        }
        login_stack.push(uid);
        cur_user = uid;
        cur_privilege = user.privilege;
        user.is_logged = true;
        user_storage.erase(uid,user);
        user_storage.insert(uid,user);
        return true;
    }
    std::cerr << user.password.toString() << std::endl;
    if (password.empty() || !Tool::isValidPassword(password) || user.password != MakeArray(password))
    {
        Tool::printInvalid();
        return false;
    }
    login_stack.push(uid);
    cur_user = uid;
    cur_privilege = user.privilege;
    user.is_logged = true;
    user_storage.erase(uid,user);
    user_storage.insert(uid,user);
    return true;
}
bool UserManager::logout()
{
    if (cur_privilege == 0)
    {
        Tool::printInvalid();
        return false;
    }
    if (login_stack.empty())
    {
        Tool::printInvalid();
        return false;
    }
    MakeArray uid(login_stack.top());
    login_stack.pop();
    std::vector<UserInfo> infos = user_storage.find(uid);
    if (infos.empty())
    {
        Tool::printInvalid();
        return false;
    }
    if (!infos.empty())
    {
        UserInfo& user = infos[0];
        user.is_logged = false;
        user_storage.erase(uid,user);
        user_storage.insert(uid,user);
    }
    if (login_stack.empty())
    {
        cur_user = MakeArray("");
        cur_privilege = 0;
    }
    else
    {
        cur_user = login_stack.top();
        std::vector<UserInfo> cur_infos = user_storage.find(login_stack.top());
        if (!cur_infos.empty())
        {
            cur_privilege = cur_infos[0].privilege;
        }
    }
    return true;
}
bool UserManager::registerUser(const std::string& id,const std::string& password,const std::string& name)
{
    if (id.empty() || password.empty() || name.empty())
    {
        Tool::printInvalid();
        return false;
    }
    if (!Tool::isValidUserID(id) || !Tool::isValidPassword(password) || !Tool::isValidUsername(name))
    {
        Tool::printInvalid();
        return false;
    }
    MakeArray uid(id);
    if (!user_storage.find(uid).empty())
    {
        Tool::printInvalid();
        return false;
    }
    UserInfo new_user(password,name,1);
    new_user.is_logged = false;
    user_storage.insert(uid,new_user);
    return true;
}
bool UserManager::passwd(const std::string& id,const std::string& cur_password,const std::string& new_password)
{
    if (cur_privilege == 0)
    {
        Tool::printInvalid();
        return false;
    }
    if (id.empty() || new_password.empty())
    {
        Tool::printInvalid();
        return false;
    }
    if (!Tool::isValidUserID(id) || !Tool::isValidPassword(new_password))
    {
        Tool::printInvalid();
        return false;
    }
    MakeArray uid(id);
    std::vector<UserInfo> infos = user_storage.find(uid);
    if (infos.empty())
    {
        Tool::printInvalid();
        return false;
    }
    UserInfo& user = infos[0];
    if (cur_privilege == 7)
    {
        if (!cur_password.empty())
        {
            if (!Tool::isValidPassword(cur_password) || user.password != MakeArray(cur_password))
            {
                Tool::printInvalid();
                return false;
            }
        }
    }
    else
    {
        if (cur_password.empty() || !Tool::isValidPassword(cur_password) || user.password != MakeArray(cur_password))
        {
            Tool::printInvalid();
            return false;
        }
    }
    user.password = MakeArray(new_password);
    user_storage.erase(uid,user);
    user_storage.insert(uid,user);
    return true;
}
bool UserManager::useradd(const std::string& id,const std::string& pw,int priv,const std::string& name)
{
    if (cur_privilege == 0 || cur_privilege == 1)
    {
        Tool::printInvalid();
        return false;
    }
    if (priv >= cur_privilege)
    {
        Tool::printInvalid();
        return false;
    }
    if (id.empty() || pw.empty() || name.empty())
    {
        Tool::printInvalid();
        return false;
    }
    if (!Tool::isValidUserID(id) || !Tool::isValidPassword(pw) || !Tool::isValidPrivilege(priv) || !Tool::isValidUsername(name))
    {
        Tool::printInvalid();
        return false;
    }
    MakeArray uid(id);
    if (!user_storage.find(uid).empty())
    {
        Tool::printInvalid();
        return false;
    }
    UserInfo new_user(pw,name,priv);
    new_user.is_logged = false;
    user_storage.insert(uid,new_user);
    return true;
}
bool UserManager::deleteUser(const std::string& id)
{
    if (cur_privilege < 7)
    {
        Tool::printInvalid();
        return false;
    }
    if (id.empty() || !Tool::isValidUserID(id))
    {
        Tool::printInvalid();
        return false;
    }
    MakeArray uid(id);
    std::vector<UserInfo> infos = user_storage.find(uid);
    if (infos.empty())
    {
        Tool::printInvalid();
        return false;
    }
    UserInfo& user = infos[0];
    if (user.is_logged)
    {
        Tool::printInvalid();
        return false;
    }
    user_storage.erase(uid,user);
    return true;
}
int UserManager::getUserPrivilege() const
{
    return cur_privilege;
}
std::string UserManager::getCurUser() const
{
    return cur_user.toString();
}
bool UserManager::isLoggedIn(const std::string& id)
{
    if (id.empty()) return false;
    MakeArray uid(id);
    std::vector<UserInfo> infos = user_storage.find(uid);
    if (infos.empty()) return false;
    return infos[0].is_logged;
}
