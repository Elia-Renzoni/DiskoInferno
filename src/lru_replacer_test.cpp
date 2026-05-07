
#include <gtest/gtest.h>
#include "lru_replacer.hpp"

TEST(LRUReplacer, TestInsertion) {
    LRU lru;

    lru.Insert(10);
    lru.Insert(5);
    lru.Insert(4);
    lru.Insert(3);
    lru.Insert(33);
    lru.Insert(10); // duplicated page id

    auto lruListSize = lru.Size();
    EXPECT_EQ(lruListSize, 5);
}

TEST(LRUReplacer, TestDeletion) {
    LRU lru;

    lru.Insert(4); 
    lru.Insert(5);
    lru.Insert(3);

    auto lruListSize = lru.Size();
    EXPECT_EQ(lruListSize, 3);

    lru.Delete(4);
    lruListSize = lru.Size();
    EXPECT_EQ(lruListSize, 2);

    lru.Delete(33);
    lruListSize = lru.Size();
    EXPECT_EQ(lruListSize, 2);
}

TEST(LRUReplacer, TestEviction) {
    LRU lru;

    lru.Insert(10);
    lru.Insert(3);
    lru.Insert(1);
    lru.Insert(55);
    lru.Insert(12);
    lru.Insert(22);

    lru.Delete(1);

    auto selectedVictim =  lru.Evict();
    EXPECT_EQ(selectedVictim, 10);
    EXPECT_EQ(lru.Size(), 5);
}

TEST(LRUReplacer, TestEvictionEmpty) {
    LRU lru;
    
    lru.Insert(12);
    lru.Insert(2);

    lru.Delete(2);
    lru.Delete(12);

    auto selectedVictim = lru.Evict();
    EXPECT_EQ(selectedVictim, -1);
}
