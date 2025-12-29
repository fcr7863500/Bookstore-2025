#pragma once
#ifndef BOOKSTORE_2025_TOOL_H
#define BOOKSTORE_2025_TOOL_H
#include "makearray.h"
#include<string>
#include<vector>
#include<iomanip>
#include<sstream>
#include<cctype>
#include<climits>
#include<iostream>
#include<algorithm>
class Tool
{
public:
    //用于异常处理的函数
    static void printInvalid()
    {
        std::cout << "Invalid\n";
    }
    //用于判断ID是否合规的函数
    static bool isValidUserID(const std::string& str)
    {
        if (str.empty())  return false;
        if (str.size() > 30) return false;
        for (char c : str)
        {
            if (!isalnum(c) && c != '_') return false;
        }
        return true;
    }
    //用于判断密码是否合规的函数
    static bool isValidPassword(const std::string& str)
    {
        if (str.empty())  return true;
        if (str.size() > 30) return false;
        for (char c : str)
        {
            if (!isalnum(c) && c != '_') return false;
        }
        return true;
    }
    //用于判断用户名是否合规
    static bool isValidUsername(const std::string& str)
    {
        if (str.empty() || str.size() > 30) return false;
        for (char c : str)
        {
            if (iscntrl(c) || c == 32) return false;
        }
        return true;
    }
    //用于判断权限是否合规
    static bool isValidPrivilege(int priv)
    {
        return (priv == 1 || priv == 3 || priv == 7);
    }
    //用于判断ISBN是否合规
    static bool isValidISBN(const std::string& str)
    {
        if (str.empty() || str.size() > 20) return false;
        for (char c : str)
        {
            if (iscntrl(c) || c == 32) return false;
        }
        return true;
    }
    //用于判断书名是否合规
    static bool isValidBookname(const std::string& str)
    {
        if (str.empty() || str.size() > 60) return false;
        for (char c : str)
        {
            if (iscntrl(c) || c == 32 || c == '"') return false;
        }
        return true;
    }
    //用于判断作者名是否合规
    static bool isValidAuthor(const std::string& str)
    {
        return isValidBookname(str);
    }
    //
    static bool isValidKeyword(const std::string& str)
    {
        if (str.empty() || str.size() > 60) return false;
        std::vector<std::string> parts = split(str,'|');
        for (const auto& part : parts)
        {
            if (part.empty()) return false;
            for (char c : part)
            {
                if (iscntrl(c) || c == 32 || c == '"') return false;
            }
        }
        std::vector<std::string> unique_parts;//用于判断关键词是否有重复,确保没有重复部分
        for (const auto& part : parts)
        {
            if (std::find(unique_parts.begin(), unique_parts.end(), part) != unique_parts.end())
            {
                return false;
            }
            unique_parts.push_back(part);
        }
        return true;
    }
    static bool isValidQuantity(const std::string& str,int& out_val)
    {
        out_val = 0;
        if (str.empty() || str.size() > 10) return false;
        if (str[0] == 0 && str[1] == 0) return false;
        for (char c : str)
        {
            if (!isdigit(c)) return false;
        }
        long long val = std::stoll(str);
        if (val < 0 || val > INT_MAX) return false;
        out_val = static_cast<int>(val);
        return true;
    }
    static bool isValidPrice(const std::string& str,double& out_val)
    {
        out_val = 0.0;
        if (str.empty() || str.size() > 13) return false;
        if (str[0] == 0 && str[1] == 0) return false;
        int dot_cnt = 0;
        for (char c : str)
        {
            if (c == '.')
            {
                dot_cnt++;
                if (dot_cnt > 1) return false;
            }
            else if (!isdigit(c))
            {
                return false;
            }
        }
        double val = std::stod(str);
        if (val < 0) return false;
        out_val = static_cast<double>(val);
        return true;
    }
    //一些字符串处理函数
    //分割字符串（尤其用于书籍的关键词部分拆分’|‘）
    static std::vector<std::string> split(const std::string& str, char target)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, target))
        {
            tokens.push_back(token);
        }
        return tokens;
    }
    //用于清除空格的函数
    static std::string RemoveBlank(const std::string& str)
    {
        size_t begin_pos = 0;
        while (str[begin_pos] == ' ')
        {
            begin_pos++;
        }
        if (begin_pos == str.size())
        {
            return "";
        }
        size_t end_pos = str.size() - 1;
        while (str[end_pos] == ' ')
        {
            end_pos--;
        }
        return str.substr(begin_pos, end_pos - begin_pos + 1);
    }
    //用于分解命令行输入指令的函数
    static std::vector<std::string> splitCommand(const std::string& str)
    {
        std::cerr << "split cmd " << str <<std::endl;
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (ss >> token)
        {
            std::cerr << "token " << token << std::endl;
            tokens.push_back(token);
        }
        return tokens;
    }
    //用于输出两位小数
    static std::string TwoDouble(double num)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << num;
        return oss.str();
    }
    //用于去除指令中的双引号
    static std::string RemoveQuoted(const std::string& str)
    {
        if (str.size() >= 2 && str.front() == '"' && str.back() == '"')
        {
            return str.substr(1, str.size() - 2);
        }
        return str;
    }
    //排除查询中可能出现keyword重复的情况
    static bool isSingleKeyWord(const std::string& str)
    {
        for (int i = 0; i < (int)str.size(); i++)
        {
            if (str[i] == '|')
            {
                return false;
            }
        }
        return true;
    }
};
#endif //BOOKSTORE_2025_TOOL_H