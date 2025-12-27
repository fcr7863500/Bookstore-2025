#include"../include/usermanager.h"
#include"../include/bookmanager.h"
#include"../include/logmanager.h"
#include"../include/cmdparser.h"
#include<string>
#include<iostream>
int main()
{
    UserManager um;
    BookManager bm;
    LogManager lm(bm,um);
    CommandParser parser(um,bm,lm);
    std::string line;
    while (std::getline(std::cin,line))
    {
        parser.Execute(line);
    }
    return 0;
}