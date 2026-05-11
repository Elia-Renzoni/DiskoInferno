
#include "lru_replacer.hpp"

LRU::LRU() {};

/**
 * execution example:
 * [1][2][3][4][5][6] (fid=7, pin_count == 0)
 * [7][1][2][3][4][5][6]
 */
void LRU::Insert(int fid) {
    if (fid < 0) return; 
    std::scoped_lock lock(LRU::latch_);

    if (LRU::lookupMap_.count(fid)) return;

    LRU::lruList_.push_front(fid);
    LRU::lookupMap_[fid] = LRU::lruList_.begin();
};

/**
 * execution example:
 * [7][1][2][3][4][5][6] (fid=6, pin_count > 0)
 * [7][1][2][3][4][5]
 *
 */
void LRU::Delete(int fid) {
    if (fid < 0) return;
    std::scoped_lock lock(LRU::latch_);

    if (!LRU::lookupMap_.count(fid)) return;
    
    auto it = LRU::lookupMap_[fid];
    LRU::lruList_.erase(it);
    LRU::lookupMap_.erase(fid);
};

/**
 * execution example
 * [7][1][2][3][4][5]
 *  5
 */
int LRU::Evict() {
    std::scoped_lock lock(LRU::latch_);

    if (LRU::lruList_.empty()) return -1;
    return LRU::lruList_.back();
};

int LRU::Size() {
    std::scoped_lock lock(LRU::latch_);
    return LRU::lruList_.size();
};
