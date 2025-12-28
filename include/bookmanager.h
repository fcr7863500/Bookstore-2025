#pragma once
#ifndef BOOKSTORE_2025_BOOKMANAGER_H
#define BOOKSTORE_2025_BOOKMANAGER_H
#include"file storage.h"
#include"makearray.h"
#include"tool.h"
#include<vector>
#include<string>
#include<map>

struct BookInfo
{
    MakeArray isbn;
    MakeArray name;
    MakeArray author;
    MakeArray keywords;
    double price;
    int quantity;
    BookInfo() : price(0.0), quantity(0) {}
    BookInfo(const std::string& isbn,const std::string& name,const std::string& author,const std::string& keywords,double price,int quantity):
    isbn(isbn),name(name),author(author),keywords(keywords),price(price),quantity(quantity) {}
    bool operator<(const BookInfo& other) const
    {
        if (isbn != other.isbn) return (isbn < other.isbn);
        if (name != other.name) return (name < other.name);
        if (author != other.author) return (author < other.author);
        if (keywords != other.keywords) return (keywords < other.keywords);
        if (price != other.price) return (price < other.price);
        return (quantity < other.quantity);
    }
    bool operator==(const BookInfo& other) const
    {
        return(isbn == other.isbn);
    }
    bool operator>(const BookInfo& other) const
    {
        if (isbn != other.isbn) return (isbn > other.isbn);
        if (name != other.name) return (name > other.name);
        if (author != other.author) return (author > other.author);
        if (keywords != other.keywords) return (keywords > other.keywords);
        if (price != other.price) return (price > other.price);
        return (quantity > other.quantity);
    }
    bool operator<=(const BookInfo& other) const
    {
        return !(*this > other);
    }
    bool operator>=(const BookInfo& other) const
    {
        return !(*this < other);
    }
    bool operator!=(const BookInfo& other) const
    {
        return !(*this == other);
    }
};
//图书交易记录
struct TradeRecord
{
    MakeArray isbn;
    int quantity;
    double amount;
    MakeArray opt_id;
    long long time_cord;
    TradeRecord():quantity(0),amount(0.0),time_cord(0){}
    TradeRecord(const std::string& isbn,int qty,double amt,const std::string& id,long long time):
    isbn(isbn),quantity(qty),amount(amt),opt_id(id),time_cord(time){}
    bool operator<(const TradeRecord& other) const
    {
        return time_cord < other.time_cord;
    }
    bool operator==(const TradeRecord& other) const
    {
        return time_cord == other.time_cord;
    }
    bool operator>(const TradeRecord& other) const
    {
        return time_cord > other.time_cord;
    }
    bool operator<=(const TradeRecord& other) const
    {
        return !(*this > other);
    }
    bool operator>=(const TradeRecord& other) const
    {
        return !(*this < other);
    }
    bool operator!=(const TradeRecord& other) const
    {
        return !(*this == other);
    }
};
//图书管理类
class BookManager
{
private:
    LinkedBlock<MakeArray,BookInfo> book_storage;
    LinkedBlock<MakeArray,MakeArray> name_find;
    LinkedBlock<MakeArray,MakeArray> author_find;
    LinkedBlock<MakeArray,MakeArray> keywords_find;
    LinkedBlock<MakeArray,TradeRecord> trade_storage;
    MakeArray selected_book;
    MakeArray cur_operator;
    int cur_privilege;
    long long trade_id_count;
    void initTradeCounter();

public:
    BookManager():book_storage("books.dat"),name_find("name.data"),author_find("author.data"),keywords_find("keywords.data"),
    trade_storage("trades.dat"),cur_privilege(0),cur_operator(MakeArray("")),trade_id_count(0)
    {
        initTradeCounter();
    }
    void setCurOperator(const std::string& id,int priv)
    {
        cur_privilege = priv;
        cur_operator = MakeArray(id);
    }
    bool show(const std::string& type,const std::string& val);
    bool buy(const std::string& isbn,int quantity);
    bool select(const std::string& isbn);
    bool modify(const std::map<const std::string,std::string>& target);
    bool import(int quantity,double total_cost);
    std::vector<TradeRecord> showFinance(int count = -1);
    std::vector<TradeRecord> showAll();
    std::string getSelectBook();
};
#endif //BOOKSTORE_2025_BOOKMANAGER_H