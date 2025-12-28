#include"../include/logmanager.h"
#include<iostream>
#include<iomanip>
#include<algorithm>
//初始化计数器
void LogManager::initOpCount()
{
    std::vector<OperationLog> all_logs = op_log_storage.getAll();
    if (!all_logs.empty())
    {
        std::sort(all_logs.begin(),all_logs.end());
        op_id_count = all_logs.back().time_cord + 1;
    }
    else
    {
        op_id_count = 1;
    }
}
//实现记录操作日志函数
void LogManager::logOperation(const std::string& op_id,const std::string& op_type,const std::string& op_target)
{
    OperationLog log(op_id,op_type,op_target,op_id_count++);
    op_log_storage.insert(MakeArray(op_id),log);
}
//展示财务记录函数（必做）
bool LogManager::Showfinance(int count)
{
    if (user_manager.getUserPrivilege() != 7)
    {
        Tool::printInvalid();
        return false;
    }
    std::vector<TradeRecord> trades = book_manager.showFinance(count);
    double income = 0.0; double outcome = 0;
    std::cerr << count << " a" << std::endl;
    std::cerr << trades.size() << " b" << std::endl;
    if (count > (int)trades.size())
    {
        std::cerr << count << ">" << trades.size() << " c" << std::endl;
        Tool::printInvalid();
        return false;
    }
    if (count == 0)
    {
        std::cout << '\n';
         return true;
    }
    for (auto& trade : trades)
    {
        std::cerr << "amount " << trade.amount << std::endl;
        if (trade.amount > 0)
        {
            income += trade.amount;
        }
        else
        {
            outcome += -trade.amount;
        }
    }
    std::cout << "+ " << Tool::TwoDouble(income) << " - " << Tool::TwoDouble(outcome) << '\n';
    return true;
}
//财务报表
bool LogManager::reportFinance()
{
    if (user_manager.getUserPrivilege() != 7)
    {
        Tool::printInvalid();
        return false;
    }
    double income = 0.0; double outcome = 0.0;
    std::vector<TradeRecord> all_trades = book_manager.showAll();
    std::cout <<"~~~~~~~~~~~~~~~~~~~~财务报表~~~~~~~~~~~~~~~~~~~~\n";
    std::cout<<'\t'<<"ISBN"<<'\t'<<"数量"<<'\t'<<"金额"<<'\t'<<"操作人"<<'\n';
    std::cout <<"-----------------------------------------------\n";
    for (auto& trade : all_trades)
    {
        std::cout<<'\t'<<trade.isbn.toString()<<'\t'<<trade.quantity<<'\t'<<Tool::TwoDouble(trade.amount)<<'\t'<<trade.opt_id.toString()<<'\n';
        if (trade.amount > 0)
        {
            income += trade.amount;
        }
        else
        {
            outcome += -trade.amount;
        }
    }
    std::cout <<"-----------------------------------------------\n";
    std::cout << "总收入："<<Tool::TwoDouble(income)<<'\n';
    std::cout << "总支出："<<Tool::TwoDouble(outcome)<<'\n';
    std::cout << "总利润："<<Tool::TwoDouble(income - outcome)<<'\n';
    std::cout <<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    return true;
}
bool LogManager::reportEmployee()
{
    if (user_manager.getUserPrivilege() != 7)
    {
        Tool::printInvalid();
        return false;
    }
    std::vector<OperationLog> all_logs = op_log_storage.getAll();
    std::sort(all_logs.begin(),all_logs.end());
    std::cout <<"~~~~~~~~~~~~~~~~~~~~员工工作情况表~~~~~~~~~~~~~~~~~~~~\n";
    std::cout <<'\t'<<"操作时间"<<'\t'<<"操作人ID"<<'\t'<<"操作类型"<<'\t'<<"操作对象"<<'\n';
    std::cout <<"----------------------------------------------------\n";
    for (auto& log : all_logs)
    {
        std::cout <<'\t'<<log.time_cord<<'\t'<<log.op_id.toString()<<'\t'<<log.op_type.toString()<<'\t'<<log.op_target.toString()<<'\n';
    }
    std::cout <<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    return true;
}
bool LogManager::log()
{
    if (user_manager.getUserPrivilege() != 7)
    {
        Tool::printInvalid();
        return false;
    }
    std::vector<TradeRecord> all_trades = book_manager.showAll();
    std::vector<OperationLog> all_logs = op_log_storage.getAll();
    std::cout<<"********************系统日志********************\n";
    std::cout<<"【财务报告】：\n";
    reportFinance();
    std::cout<<"【员工操作报告】：\n";
    reportEmployee();
    std::cout<<'\n';
    std::cout<<"************************************************\n";
    return true;
}