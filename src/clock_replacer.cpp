
#include <mutex>

#include "clock_replacer.hpp"

/**
 * execution example:
 * [1][2][3][4][5] (new: 6)
 * [1][2][3][4][5][6]
 */
void ClockReplacer::Insert(int pid) {
    if (pid < 0) return;

    auto entry = ClockReplacer::ClockEntry{
        .pageID = pid,
        .roundFlag = false,
    };

    std::scoped_lock lock(ClockReplacer::latch_);
    if (ClockReplacer::lookupMap_.count(pid)) return;

    ClockReplacer::clockBuffer_.push_back(entry);

    ClockReplacer::lookupMap_[pid] = ClockReplacer::clockBuffer_.begin();
};

/**
 *
 *
 */
void ClockReplacer::Delete(int pid) {
    if (pid < 0) return;

    std::scoped_lock lock(ClockReplacer::latch_);
    if (!ClockReplacer::lookupMap_.count(pid)) return;

    auto it = ClockReplacer::lookupMap_[pid];
    ClockReplacer::clockBuffer_.erase(it);
    ClockReplacer::lookupMap_.erase(pid);
};

/**
 * execution example:
 * [1 - false][2 - false][3 - false][4 - false][5 - false] <- before round #1
 * [1 - true][2 - true][3 - true][4 - true][5 - true] <- after round #1
 * second round -> eviction [1]
 */
int ClockReplacer::Evict() {
    std::scoped_lock lock(ClockReplacer::latch_);

    ClockEntry victim;
    victim.pageID = -1;

    constexpr int rounds = 2;
    for (int round = 0; round < rounds; round++) {
          for (auto &entry : ClockReplacer::clockBuffer_) {
              if (!entry.roundFlag && !round) entry.roundFlag = true;
              else if (entry.roundFlag && round) {
                  victim = entry;
                  break;
              }
          }
    }

    if (victim.pageID != -1) {
        auto it = ClockReplacer::lookupMap_[victim.pageID];
        ClockReplacer::clockBuffer_.erase(it);
        ClockReplacer::lookupMap_.erase(victim.pageID);
    } 

    return victim.pageID;
};

int ClockReplacer::Size() {
    std::scoped_lock lock(ClockReplacer::latch_);
    return ClockReplacer::clockBuffer_.size();
};
