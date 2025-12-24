#pragma once
#ifndef BOOKSTORE_2025_FILE_STORAGE_H
#define BOOKSTORE_2025_FILE_STORAGE_H
#include<iostream>
#include<fstream>
#include<vector>
#include <cstring>
#include<string>
#include<array>
#include<algorithm>
#include<limits>

template<typename K,typename V> class LinkedBlock
{
private:
    struct KVmap
    {
        K index_;
        V value_;

        KVmap() : index_(),value_() {}

        KVmap(const K& index,const V& val):index_(index),value_(val){}

        bool operator<(const KVmap &other) const
        {
            if (index_ != other.index_)  return index_ < other.index_;
            return value_ < other.value_;
        }

        bool operator>(const KVmap &other) const
        {
            if (index_ != other.index_)  return index_ > other.index_;
            return value_ > other.value_;
        }

        bool operator==(const KVmap &other) const
        {
            return index_ == other.index_ && value_ == other.value_;
        }

        bool operator!=(const KVmap &other) const
        {
            return !(*this == other);
        }

        bool operator>=(const KVmap &other) const
        {
            return !(*this < other);
        }

        bool operator<=(const KVmap &other) const
        {
            return !(*this > other);
        }
    };

    class Block
    {
    private:
        KVmap first_;
        KVmap last_;
        int next_offset;
        int pre_offset;
        int offset;
        int num;
        static const int MAX_NUM = 500;
        KVmap data[MAX_NUM + 1];

    public:
        Block():next_offset(-1), pre_offset(-1), offset(-1), num(0){}

        void insert(const KVmap &kv);
        void erase(const KVmap &kv);
        void collectValues(const K &index, std::vector<V> &values);

        const KVmap& getFirst() const { return first_; }
        const KVmap& getLast() const { return last_; }
        int getOffset() const { return offset; }
        int getNextOffset() const {return next_offset;}
        int getPreOffset() const {return pre_offset;}
        int getNum() const { return num; }

        void setFirst(const KVmap& m) { first_ = m; }
        void setLast(const KVmap& m) { last_ = m; }
        void setOffset(int b) { offset = b; }
        void setNextOffset(int b) {next_offset = b;}
        void setPreOffset(int b) {pre_offset = b;}
        void setNum(int n) { num = n; }

        const KVmap* getData() const { return data; }
        KVmap* getData() { return data; }
        friend class LinkedBlock;
    };
private:
    const int block_size_;
    std::string kvmap_file;
    int num_of_blocks;
    int head_offset;
    std::fstream file;

    int findBlock(const KVmap &kv);

    Block readBlock(int offset)
    {
        Block block;
        if (!file.is_open()) return block;
        file.seekg(offset);
        file.read(reinterpret_cast<char*>(&block.first_), sizeof(KVmap));
        file.read(reinterpret_cast<char*>(&block.last_), sizeof(KVmap));
        file.read(reinterpret_cast<char*>(&block.next_offset), sizeof(int));
        file.read(reinterpret_cast<char*>(&block.pre_offset), sizeof(int));
        file.read(reinterpret_cast<char*>(&block.offset), sizeof(int));
        file.read(reinterpret_cast<char*>(&block.num), sizeof(int));
        file.read(reinterpret_cast<char*>(block.data), sizeof(KVmap)*(Block::MAX_NUM + 1));
        return block;
    }

    void writeBlock(int offset, const Block &block)
    {
        if (!file.is_open()) return;
        file.seekp(offset);
        file.write(reinterpret_cast<const char*>(&block.first_), sizeof(KVmap));
        file.write(reinterpret_cast<const char*>(&block.last_), sizeof(KVmap));
        file.write(reinterpret_cast<const char*>(&block.next_offset), sizeof(int));
        file.write(reinterpret_cast<const char*>(&block.pre_offset), sizeof(int));
        file.write(reinterpret_cast<const char*>(&block.offset), sizeof(int));
        file.write(reinterpret_cast<const char*>(&block.num), sizeof(int));
        file.write(reinterpret_cast<const char*>(block.data), sizeof(KVmap)*(Block::MAX_NUM + 1));
    }

    int createNewBlock()
    {
        Block new_block;
        int new_offset = num_of_blocks * block_size_;
        new_block.setOffset(new_offset);
        new_block.setNextOffset(-1);
        new_block.setPreOffset(-1);
        writeBlock(new_offset, new_block);
        num_of_blocks++;
        return new_offset;
    }

    void readBlockMeta(int offset, KVmap& first, KVmap& last, int& next, int& num)
    {
        if (!file.is_open()) return;
        file.seekg(offset);
        file.read(reinterpret_cast<char*>(&first), sizeof(KVmap));
        file.read(reinterpret_cast<char*>(&last), sizeof(KVmap));
        file.read(reinterpret_cast<char*>(&next), sizeof(int));
        file.ignore(sizeof(int));
        file.ignore(sizeof(int));
        file.read(reinterpret_cast<char*>(&num), sizeof(int));
    }

    void writeBlockMeta(int offset, const KVmap& first, const KVmap& last, int next, int pre, int num)
    {
        if (!file.is_open()) return;
        file.seekp(offset);
        file.write(reinterpret_cast<const char*>(&first), sizeof(KVmap));
        file.write(reinterpret_cast<const char*>(&last), sizeof(KVmap));
        file.write(reinterpret_cast<const char*>(&next), sizeof(int));
        file.write(reinterpret_cast<const char*>(&pre), sizeof(int));
        file.ignore(sizeof(int));
        file.write(reinterpret_cast<const char*>(&num), sizeof(int));
    }

public:
    LinkedBlock(const std::string &kvmap_file)
        : kvmap_file(kvmap_file), block_size_(sizeof(Block)), num_of_blocks(0), head_offset(-1)
    {
        file.open(kvmap_file, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            file.clear();
            file.open(kvmap_file, std::ios::out | std::ios::binary);
            file.close();
            file.open(kvmap_file, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
        }

        int file_size = file.tellg();
        num_of_blocks = (file_size == 0) ? 0 : (file_size / block_size_);
        head_offset = (num_of_blocks == 0) ? createNewBlock() : 0;
    }

    ~LinkedBlock()
    {
        if (file.is_open())
        {
             file.flush();
             file.close();
        }
    }

    void insert(const K &index, const V &value);
    void erase(const K &index, const V &value);
    std::vector<V> find(const K &index);
};

template<typename K,typename V>
void LinkedBlock<K,V>::Block::insert(const KVmap &kv)
{
    int pos = std::lower_bound(data, data + num, kv) - data;
    if (pos < num && data[pos] == kv) return;

    if (num == 0)
    {
        first_ = kv;
        last_ = kv;
        data[0] = kv;
        num = 1;
    }
    else
    {
        for (int i = num; i > pos; --i)
        {
            data[i] = data[i - 1];
        }
        data[pos] = kv;
        num++;
        if (kv < first_) first_ = kv;
        if (kv > last_) last_ = kv;
    }
}

template<typename K,typename V>
void LinkedBlock<K,V>::Block::erase(const KVmap &kv)
{
    int pos = std::lower_bound(data, data + num, kv) - data;
    if (pos >= num || data[pos] != kv) return;

    for (int i = pos; i < num - 1; ++i)
    {
        data[i] = data[i + 1];
    }
    num--;

    if (num == 0)
    {
        first_ = KVmap();
        last_ = KVmap();
    }
    else
    {
        first_ = data[0];
        last_ = data[num - 1];
    }
}

template<typename K,typename V>
void LinkedBlock<K,V>::Block::collectValues(const K &index, std::vector<V> &values)
{
    KVmap min_kv(index, std::numeric_limits<V>::min());
    KVmap max_kv(index, std::numeric_limits<V>::max());
    for (int i = 0; i < num; ++i)
    {
        if (data[i] >= min_kv && data[i] <= max_kv)
        {
            values.push_back(data[i].value_);
        }
    }
}

template<typename K,typename V>
int LinkedBlock<K,V>::findBlock(const KVmap &kv)
{
    int cur_offset = head_offset;
    int target_offset = cur_offset;
    while (cur_offset != -1)
    {
        KVmap first, last;
        int next, num;
        readBlockMeta(cur_offset, first, last, next, num);
        if (num == 0)
        {
            target_offset = cur_offset;
            break;
        }
        if (last >= kv)
        {
            target_offset = cur_offset;
            break;
        }
        target_offset = cur_offset;
        cur_offset = next;
    }
    return target_offset;
}

template<typename K,typename V>
void LinkedBlock<K,V>::insert(const K &index, const V &value)
{
    KVmap kv(index, value);
    int block_offset = findBlock(kv);
    Block block = readBlock(block_offset);

    block.insert(kv);
    writeBlock(block_offset, block);

    if (block.getNum() >= Block::MAX_NUM)
    {
        int new_block_offset = createNewBlock();
        Block new_block = readBlock(new_block_offset);
        int mid = Block::MAX_NUM / 2;

        block.setNum(mid);
        block.setLast(block.getData()[mid - 1]);
        for (int i = 0; i < Block::MAX_NUM - mid; ++i)
        {
            new_block.getData()[i] = block.getData()[mid + i];
        }
        new_block.setNum(Block::MAX_NUM - mid);
        new_block.setFirst(new_block.getData()[0]);
        new_block.setLast(new_block.getData()[new_block.getNum() - 1]);

        int old_next = block.getNextOffset();
        block.setNextOffset(new_block_offset);
        new_block.setPreOffset(block_offset);
        new_block.setNextOffset(old_next);

        writeBlock(block_offset, block);
        writeBlock(new_block_offset, new_block);
        if (old_next != -1)
        {
            Block old_next_block = readBlock(old_next);
            old_next_block.setPreOffset(new_block_offset);
            writeBlock(old_next, old_next_block);
        }
    }
}

template<typename K,typename V>
void LinkedBlock<K,V>::erase(const K &index, const V &value)
{
    KVmap kv(index, value);
    int block_offset = findBlock(kv);
    Block block = readBlock(block_offset);

    block.erase(kv);
    writeBlock(block_offset, block);

    if (block.getNum() < Block::MAX_NUM / 4 && block.getNextOffset() != -1)
    {
        int next_block_offset = block.getNextOffset();
        Block next_block = readBlock(next_block_offset);
        int total_num = block.getNum() + next_block.getNum();

        if (total_num <= Block::MAX_NUM)
        {
            for (int i = 0; i < next_block.getNum(); ++i)
            {
                block.getData()[block.getNum() + i] = next_block.getData()[i];
            }
            block.setNum(total_num);
            block.setLast(next_block.getLast());

            int next_next_offset = next_block.getNextOffset();
            block.setNextOffset(next_next_offset);

            writeBlock(block_offset, block);
            if (next_next_offset != -1)
            {
                Block next_next_block = readBlock(next_next_offset);
                next_next_block.setPreOffset(block_offset);
                writeBlock(next_next_offset, next_next_block);
            }
        }
    }
}

template<typename K,typename V>
std::vector<V> LinkedBlock<K,V>::find(const K &index)
{
    std::vector<V> values;
    KVmap min_kv(index, std::numeric_limits<V>::min());
    KVmap max_kv(index, std::numeric_limits<V>::max());
    int cur_offset = head_offset;

    while (cur_offset != -1)
    {
        KVmap first, last;
        int next, num;
        readBlockMeta(cur_offset, first, last, next, num);

        if (num == 0)
        {
            cur_offset = next;
            continue;
        }
        if (first > max_kv) break;
        if (last >= min_kv)
        {
            Block block = readBlock(cur_offset);
            block.collectValues(index, values);
        }
        cur_offset = next;
    }
    return values;
}
#endif //BOOKSTORE_2025_FILE_STORAGE_H