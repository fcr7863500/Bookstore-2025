#include"../include/bookmanager.h"
#include<iostream>
#include<algorithm>
#include<ctime>

void BookManager::initTradeCounter()
{
    std::vector<TradeRecord> all_trades = trade_storage.getAll();
    if (! all_trades.empty())
    {
        std::sort(all_trades.begin(),all_trades.end());
        trade_id_count = all_trades.back().time_cord + 1;
    }
    else
    {
        trade_id_count = 1;
    }
}
//有可能超时的一个函数
bool BookManager::show(const std::string& type,const std::string& value)
{
    if (cur_privilege < 1)
    {
        Tool::printInvalid();
        return false;
    }
    std::vector<BookInfo> all_books = book_storage.getAll();
    std::vector<BookInfo> target_books;
    if (type.empty())
    {
        target_books = all_books;
    }
    else
    {
        if (value.empty())
        {
            Tool::printInvalid();
            return false;
        }
        if (type == "ISBN")
        {
            if (!Tool::isValidISBN(value))
            {
                Tool::printInvalid();
                return false;
            }
            MakeArray isbn_key(value);
            target_books = book_storage.find(isbn_key);
        }
        else if (type == "name")
        {
            std::string name_val = Tool::RemoveQuoted(value);
            if (!Tool::isValidBookname(name_val))
            {
                Tool::printInvalid();
                return false;
            }
            for (auto &book : all_books)
            {
                if (book.name == name_val)
                {
                    target_books.push_back(book);
                }
            }
        }
        else if (type == "author")
        {
            std::string author_val = Tool::RemoveQuoted(value);
            if (!Tool::isValidAuthor(author_val))
            {
                Tool::printInvalid();
                return false;
            }
            for (auto &book : all_books)
            {
                if (book.author == author_val)
                {
                    target_books.push_back(book);
                }
            }
        }
        else if (type =="keyword")
        {
            std::string keyword_val = Tool::RemoveQuoted(value);
            if (!Tool::isValidKeyword(keyword_val))
            {
                Tool::printInvalid();
                return false;
            }
            std::vector<std::string> cmd = Tool::split(keyword_val,'|');
            if (cmd.size() > 1)
            {
                Tool::printInvalid();
                return false;
            }
            for (auto &book : all_books)
            {
                std::string book_keyword = book.keywords.toString();
                std::vector<std::string> parts = Tool::split(book_keyword, '|');
                if (std::find(parts.begin(), parts.end(), keyword_val) != parts.end())
                {
                    target_books.push_back(book);
                }
            }
        }
        else
        {
            Tool::printInvalid();
            return false;
        }
    }
    std::sort(target_books.begin(),target_books.end());
    if (target_books.empty())
    {
        std::cout << '\n';
    }
    else
    {
        for (auto &book : target_books)
        {
            std::cerr << "isbn: " << book.isbn.toString() << '\n';
            std::cerr << "name: " << book.name.toString() << '\n';
            std::cout << book.isbn.toString() << '\t' << book.name.toString() << '\t' << book.author.toString() << '\t' << book.keywords.toString() << '\t' << Tool::TwoDouble(book.price) << '\t' << book.quantity << '\n';
        }
    }
    return true;
}
//购买函数实现
bool BookManager::buy(const std::string& isbn,int quantity)
{
    if (cur_privilege < 1 || quantity <= 0 || !Tool::isValidISBN(isbn))
    {
        std::cerr << "bad cons" << std::endl;
        Tool::printInvalid();
        return false;
    }
    MakeArray isbn_key(isbn);
    std::vector<BookInfo> target_books = book_storage.find(isbn_key);
    if (target_books.empty())
    {
        std::cerr << "not found" << std::endl;
        Tool::printInvalid();
        return false;
    }
    BookInfo& book = target_books[0];
    if (book.quantity < quantity)
    {
        std::cerr << "not enough" << std::endl;
        Tool::printInvalid();
        return false;
    }
    double total = book.price * quantity;
    std::cout << Tool::TwoDouble(total) << '\n';
    book.quantity -= quantity;
    book_storage.erase(isbn_key,book);
    if (book.quantity > 0)
    {
        book_storage.insert(isbn_key,book);
    }
    TradeRecord trade(isbn,-quantity,total,cur_operator.toString(),trade_id_count++);
    trade_storage.insert(isbn_key,trade);
    return true;
}
//选择图书的函数实现
bool BookManager::select(const std::string& isbn)
{
    if (cur_privilege < 3 || !Tool::isValidISBN(isbn))
    {
        Tool::printInvalid();
        return false;
    }
    MakeArray isbn_key(isbn);
    std::vector<BookInfo> target_books = book_storage.find(isbn_key);
    if (target_books.empty())
    {
        BookInfo new_book;
        new_book.isbn = isbn; new_book.quantity = 0;new_book.price = 0.0; new_book.author = "";new_book.keywords = "";new_book.name = "";
        book_storage.insert(isbn_key,new_book);
    }
    selected_book = isbn_key;
    return true;
}
//修改图书信息的函数实现
bool BookManager::modify(const std::map<const std::string,std::string>& target)
{
    if (cur_privilege < 3 || selected_book.isEmpty() || target.empty())
    {
        Tool::printInvalid();
        return false;
    }
//检查指令参数是否有重复
    std::vector<std::string> cmds;
    std::vector<std::string> vals;
    for (auto &item : target)
    {
        cmds.push_back(item.first);
        vals.push_back(item.second);
    }
    std::sort(cmds.begin(),cmds.end());
    for (int i = 1; i < cmds.size(); ++i)
    {
        if (cmds[i] == cmds[i - 1])
        {
            Tool::printInvalid();
            return false;
        }
    }
    //检查是否有参数为空
    for (int i = 0;i < vals.size();++i)
    {
        if (vals[i].empty())
        {
            Tool::printInvalid();
            return false;
        }
    }
    MakeArray isbn_key = selected_book;
    std::vector<BookInfo> target_books = book_storage.find(isbn_key);
    std::vector<BookInfo> all_books = book_storage.getAll();
    if (target_books.empty())
    {
        Tool::printInvalid();
        return false;
    }
    BookInfo book = target_books[0];
    BookInfo tmp_book;
    tmp_book.isbn = book.isbn;tmp_book.name = name;tmp_book.key_words = book.Key_words;tmp_book.price = book.price;tmp_book.quantity = book.quantity;tmp_book.author = book.author;
    book_storage.erase(isbn_key,book);
    for (auto &item : target)
    {
        std::string key = item.first;
        std::string value = Tool::RemoveQuoted(item.second);
        if (key == "ISBN")
        {
            if (!Tool::isValidISBN(value) || value == book.isbn.toString())
            {
                Tool::printInvalid();
                book_storage.insert(isbn_key,book);
                return false;
            }
            for(auto bookss : all_books){
                if(value == bookss.isbn.toString()){
                     Tool::printInvalid();
                book_storage.insert(isbn_key,book);
                return false;
                }
            }
            tmp_book.isbn = MakeArray(value);
        }
        else if (key == "name")
        {
            if (!Tool::isValidBookname(value))
            {
                Tool::printInvalid();
                book_storage.insert(isbn_key,book);
                return false;
            }
            tmp_book.name = MakeArray(value);
        }
        else if (key == "author")
        {
            if (!Tool::isValidAuthor(value))
            {
                Tool::printInvalid();
                book_storage.insert(isbn_key,book);
                return false;
            }
            tmp_book.author = MakeArray(value);
        }
        else if (key == "keyword")
        {
            if (!Tool::isValidKeyword(value))
            {
                Tool::printInvalid();
                book_storage.insert(isbn_key,book);
                return false;
            }
            tmp_book.keywords = MakeArray(value);
        }
        else if (key == "price")
        {
            double price_val;
            if (!Tool::isValidPrice(value, price_val))
            {
                Tool::printInvalid();
                book_storage.insert(isbn_key,book);
                return false;
            }
            tmp_book.price = price_val;
        }
        else
        {
            Tool::printInvalid();
            book_storage.insert(isbn_key,book);
            return false;
        }
    }
    book_storage.insert(tmp_book.isbn,tmp_book);
    selected_book = tmp_book.isbn;
    return true;
}
//图书进货的函数实现
bool BookManager::import(int quantity,double total_cost)
{
    if (cur_privilege < 3 || selected_book.isEmpty() || quantity <= 0 || total_cost <= 0)
    {
        Tool::printInvalid();
        return false;
    }
    MakeArray isbn_key = selected_book;
    std::vector<BookInfo> target_books = book_storage.find(isbn_key);
    if (target_books.empty())
    {
        Tool::printInvalid();
        return false;
    }
    BookInfo& book = target_books[0];
    book.quantity += quantity;
    book_storage.erase(isbn_key,book);
    book_storage.insert(isbn_key,book);
    TradeRecord trade(isbn_key.toString(),quantity,-total_cost,cur_operator.toString(),trade_id_count++);
    trade_storage.insert(isbn_key,trade);
    return true;
}
std::vector<TradeRecord> BookManager::showFinance(int count)
{
    std::vector<TradeRecord> all_trades = trade_storage.getAll();
    std::sort(all_trades.begin(),all_trades.end());
    if (count == 2147483647 || count >= all_trades.size())
    {
        return all_trades;
    }
    else if (count < all_trades.size())
    {
        std::vector<TradeRecord> new_trades;
        for (int i = all_trades.size() - count; i < all_trades.size(); i++)
        {
            new_trades.push_back(all_trades[i]);
        }
        return new_trades;
    }
    else return {};
}
std::vector<TradeRecord> BookManager::showAll()
{
    std::vector<TradeRecord> all_trades = trade_storage.getAll();
    std::sort(all_trades.begin(),all_trades.end());
    return all_trades;
}
std::string BookManager::getSelectBook()
{
    return selected_book.toString();
}
