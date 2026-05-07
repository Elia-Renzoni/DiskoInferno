
#include <mutex>
#include <deque>
#include <unordered_map>

#include "replacer.hpp"

#pragma once

class LRU : public Replacer {
    public:
        LRU();
        ~LRU() = default;

        int Evict() override;
        void Delete(int pid) override;
        void Insert(int pid) override;
        int Size() override;
    private:
        std::deque<int> lruList_;
        std::unordered_map<int, std::deque<int>::iterator> lookupMap_;
        std::mutex latch_;
};
