
#include <gtest/gtest.h>
#include "clock_replacer.hpp"

TEST(ClockReplacer, TestInsertion) {
    ClockReplacer clk;

    clk.Insert(10);
    clk.Insert(22);
    clk.Insert(33);
    clk.Insert(2);

   EXPECT_EQ(clk.Size(), 4);
}

TEST(ClockReplacer, TestDeletion) {
    ClockReplacer clk;

    clk.Insert(10);
    clk.Insert(22);
    clk.Insert(33);
    clk.Insert(2);

    clk.Delete(2);

    EXPECT_EQ(clk.Size(), 3);
}

TEST(ClockReplacer, TestEviction) {
    ClockReplacer clk;

    clk.Insert(10);
    clk.Insert(22);
    clk.Insert(33);
    clk.Insert(2);

    auto victim = clk.Evict();
    EXPECT_EQ(victim, 10);
    EXPECT_EQ(clk.Size(), 3);
}


