#pragma once
#ifndef BOOKSTORE_2025_CMDPARCER_H
#define BOOKSTORE_2025_CMDPARCER_H
#include"usermanager.h"
#include"bookmanager.h"
#include"logmanager.h"
#include<string>
#include<vector>
#include<map>

class CommandParser
{
    UserManager& user_manager;
    BookManager& book_manager;
    LogManager& log_manager;

    //解析基础指令函数
    bool parseQuit(const std::vector<std::string>& cmd);
    bool parseExit(const std::vector<std::string>& cmd);
    //解析用户有关指令函数
    bool parseSu(const std::vector<std::string>& cmd);
    bool parseLogout(const std::vector<std::string>& cmd);
    bool parseRegister(const std::vector<std::string>& cmd);
    bool parsePasswd(const std::vector<std::string>& cmd);
    bool parseUseradd(const std::vector<std::string>& cmd);
    bool parseDelete(const std::vector<std::string>& cmd);
    //解析图书有关指令函数
    bool parseShow(const std::vector<std::string>& cmd);
    bool parseBuy(const std::vector<std::string>& cmd);
    bool parseSelect(const std::vector<std::string>& cmd);
    bool parseModify(const std::vector<std::string>& cmd);
    bool parseImport(const std::vector<std::string>& cmd);
    //解析日志有关指令函数
    bool parseShowfinance(const std::vector<std::string>& cmd);
    bool parseReportfinance(const std::vector<std::string>& cmd);
    bool parseReportemployee(const std::vector<std::string>& cmd);
    bool parseLog(const std::vector<std::string>& cmd);

public:
    CommandParser(UserManager& um,BookManager& bm,LogManager& lm):
    user_manager(um),book_manager(bm),log_manager(lm){}
//执行指令函数
    bool Execute(const std::string& line);
};
#endif //BOOKSTORE_2025_CMDPARCER_H