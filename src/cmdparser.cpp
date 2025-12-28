#include"../include/cmdparser.h"
#include<iostream>
//基础指令
bool CommandParser::parseQuit(const std::vector<std::string>& cmds)
{
    if (cmds.size() != 1)
    {
        Tool::printInvalid();
        return false;
    }
    exit(0);
}
bool CommandParser::parseExit(const std::vector<std::string>& cmds)
{
    if (cmds.size() != 1)
    {
        Tool::printInvalid();
        return false;
    }
    exit(0);
}
//账户指令
bool CommandParser::parseSu(const std::vector<std::string>& cmds)
{
    if (cmds.size() < 2 || cmds.size() > 3)
    {
        Tool::printInvalid();
        return false;
    }
    std::string user_id = cmds[1];
    std::string pwd = (cmds.size() == 3) ? cmds[2] : "";
    bool result = user_manager.su(user_id, pwd);
    if (result)
    {
        log_manager.logOperation(user_id,"su",user_id);
        book_manager.setCurOperator(user_manager.getCurUser());
    }
    return result;
}
bool CommandParser::parseLogout(const std::vector<std::string>& cmds)
{
    if (cmds.size() != 1)
    {
        Tool::printInvalid();
        return false;
    }
    std::string cur_user = user_manager.getCurUser();
    bool result = user_manager.logout();
    if (result)
    {
        log_manager.logOperation(cur_user,"logout",cur_user);
        book_manager.setCurOperator(user_manager.getCurUser());
    }
    return result;
}
bool CommandParser::parseRegister(const std::vector<std::string>& cmds)
{
    if (cmds.size() != 4)
    {
        Tool::printInvalid();
        return false;
    }
    std::string user_id = cmds[1];
    std::string pwd = cmds[2];
    std::string uname = cmds[3];
    bool result = user_manager.registerUser(user_id, pwd, uname);
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"register",user_id);
    }
    return result;
}
bool CommandParser::parsePasswd(const std::vector<std::string>& cmds)
{
    if (cmds.size() < 3 || cmds.size() > 4)
    {
        Tool::printInvalid();
        return false;
    }
    std::string user_id = cmds[1];
    std::string cur_pwd = (cmds.size() == 4) ? cmds[2] : "";
    std::string new_pwd = cmds.back();
    bool result = user_manager.passwd(user_id, cur_pwd, new_pwd);
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"passwd",user_id);
    }
    return result;
}
bool CommandParser::parseUseradd(const std::vector<std::string>& cmds)
{
    if (cmds.size() != 5)
    {
        Tool::printInvalid();
        return false;
    }
    std::string user_id = cmds[1];
    std::string pwd = cmds[2];
    int priv = atoi(cmds[3].c_str());
    std::string uname = cmds[4];
    bool result = user_manager.useradd(user_id,pwd,priv, uname);
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"useradd",user_id);
    }
    return result;
}
bool CommandParser::parseDelete(const std::vector<std::string>& cmds)
{
    if (cmds.size() != 2)
    {
        Tool::printInvalid();
        return false;
    }
    std::string user_id = cmds[1];
    bool result = user_manager.deleteUser(user_id);
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"delete",user_id);
    }
    return result;
}
//图书指令
bool CommandParser::parseShow(const std::vector<std::string>& cmds)
{
    if (cmds.size() < 1 || cmds.size() > 3)
    {
        Tool::printInvalid();
        return false;
    }
    if (cmds.size() >= 2 && cmds[1] == "finance")
    {
        return parseShowfinance(cmds);
    }
    std::string type = "",value = "";
    if (cmds.size() == 2)
    {
        std::string cmd = cmds[1];
        if (cmd.substr(0,6) == "-ISBN=")
        {
            type = "ISBN";
            value = cmd.substr(6);
        }
        else if (cmd.substr(0,6) == "-name=")
        {
            type = "name";
            value = cmd.substr(6);
        }
        else if (cmd.substr(0,8) == "-author=")
        {
            type = "author";
            value = cmd.substr(8);
        }
        else if (cmd.substr(0,9) == "-keyword=")
        {
            type = "keyword";
            value = cmd.substr(9);
        }
        else
        {
            Tool::printInvalid();
            return false;
        }
    }
    bool result = book_manager.show(type,value);
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"show",type + ":" + value);
    }
    return result;
}
bool CommandParser::parseBuy(const std::vector<std::string>& cmds)
{
    if (cmds.size() != 3)
    {
        Tool::printInvalid();
        return false;
    }
    std::string isbn = cmds[1];
    int quantity = 0;
    if (!Tool::isValidQuantity(cmds[2],quantity))
    {
        Tool::printInvalid();
        return false;
    }
    bool result = book_manager.buy(isbn, quantity);
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"buy",isbn + ":" + std::to_string(quantity));
    }
    return result;
}
bool CommandParser::parseSelect(const std::vector<std::string>& cmds)
{
    if (cmds.size() != 2)
    {
        Tool::printInvalid();
        return false;
    }
    std::string isbn = cmds[1];
    bool result = book_manager.select(isbn);
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"select",isbn);
    }
    return result;
}
bool CommandParser::parseModify(const std::vector<std::string>& cmds)
{
    if (cmds.size() < 2)
    {
        Tool::printInvalid();
        return false;
    }
    std::map<const std::string, std::string> modify_cmd;
    for (int i = 1; i < cmds.size(); i++)
    {
        std::string part = cmds[i];
        if (part.substr(0,6) == "-ISBN=")
        {
            modify_cmd["ISBN"] = part.substr(6);
        }
        else if (part.substr(0,6) == "-name=")
        {
            modify_cmd["name"] = part.substr(6);
        }
        else if (part.substr(0,8) == "-author=")
        {
            modify_cmd["author"] = part.substr(8);
        }
        else if (part.substr(0,9) == "-keyword=")
        {
            modify_cmd["keyword"] = part.substr(9);
        }
        else if (part.substr(0,7) == "-price=")
        {
            modify_cmd["price"] = part.substr(7);
        }
        else
        {
            Tool::printInvalid();
            return false;
        }
    }
    bool result = book_manager.modify(modify_cmd);
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"modify",book_manager.getSelectBook());
    }
    return result;
}
bool CommandParser::parseImport(const std::vector<std::string>& cmds)
{
    if (cmds.size() != 3)
    {
        Tool::printInvalid();
        return false;
    }
    int quantity = 0;
    double total_cost = 0;
    if (!Tool::isValidQuantity(cmds[1],quantity) || !Tool::isValidPrice(cmds[2],total_cost))
    {
        Tool::printInvalid();
        return false;
    }
    bool result = book_manager.import(quantity, total_cost);
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"import",book_manager.getSelectBook() +":" + std::to_string(quantity) + "for" + std::to_string(total_cost));
    }
    return result;
}
//日志指令
bool CommandParser::parseShowfinance(const std::vector<std::string>& cmds)
{
    if (cmds.size() < 2 || cmds.size() > 3)
    {
        Tool::printInvalid();
        return false;
    }
    int count = -1;
    if (cmds.size() == 3)
    {
        if (!Tool::isValidQuantity(cmds[2],count))
        {
            Tool::printInvalid();
            return false;
        }
    }
    std::cerr << "sf count: " << count << std::endl;
    bool result = log_manager.Showfinance(count);
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"show finance",std::to_string(count));
    }
    return result;
}
bool CommandParser::parseReportfinance(const std::vector<std::string>& cmds)
{
    if (cmds.size() != 2)
    {
        Tool::printInvalid();
        return false;
    }
    bool result = log_manager.reportFinance();
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"report finance","");
    }
    return result;
}
bool CommandParser::parseReportemployee(const std::vector<std::string>& cmds)
{
    if (cmds.size() != 2)
    {
        Tool::printInvalid();
        return false;
    }
    bool result = log_manager.reportEmployee();
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"report employee","");
    }
    return result;
}
bool CommandParser::parseLog(const std::vector<std::string>& cmds)
{
    if (cmds.size() != 1)
    {
        Tool::printInvalid();
        return false;
    }
    bool result = log_manager.log();
    if (result)
    {
        log_manager.logOperation(user_manager.getCurUser(),"log","");
    }
    return result;
}
bool CommandParser::Execute(const std::string& line)
{
    std::vector<std::string> cmds = Tool::splitCommand(line);
    if (cmds.empty()) return true;
    std::string init_cmd = cmds[0];
    if (init_cmd == "quit")
    {
        return parseQuit(cmds);
    }
    else if (init_cmd == "exit")
    {
        return parseExit(cmds);
    }
    else if (init_cmd == "su")
    {
        return parseSu(cmds);
    }
    else if (init_cmd == "logout")
    {
        return parseLogout(cmds);
    }
    else if (init_cmd == "register")
    {
        return parseRegister(cmds);
    }
    else if (init_cmd == "passwd")
    {
        return parsePasswd(cmds);
    }
    else if (init_cmd == "useradd")
    {
        return parseUseradd(cmds);
    }
    else if (init_cmd == "delete")
    {
        return parseDelete(cmds);
    }
    else if (init_cmd == "show")
    {
        return parseShow(cmds);
    }
    else if (init_cmd == "buy")
    {
        return parseBuy(cmds);
    }
    else if (init_cmd == "select")
    {
        return parseSelect(cmds);
    }
    else if (init_cmd == "modify")
    {
        return parseModify(cmds);
    }
    else if (init_cmd == "import")
    {
        return parseImport(cmds);
    }
    else if (init_cmd == "report")
    {
        if (cmds[1] == "finance")
        {
            return parseReportfinance(cmds);
        }
        else if (cmds[1] == "employee")
        {
            return parseReportemployee(cmds);
        }
        else
        {
            Tool::printInvalid();
            return false;
        }
    }
    else if (init_cmd == "log")
    {
        return parseLog(cmds);
    }
    else
    {
        Tool::printInvalid();
        return false;
    }
}
