
#include <gtest/gtest.h>
#include <memory>

#include "disk_manager.hpp"

TEST(DiskManager, TestInsert) {
    auto m = std::make_unique<DiskManager>("file.db");

    m->Write(10, "foo");
    m->Write(22, "bar");
    m->Write(33, "mock");
    m->Write(10, "qux");

    auto result = m->Read(10);
    EXPECT_EQ(result, "foo");

    result = m->Read(22);
    EXPECT_EQ(result, "bar");

    result = m->Read(33);
    EXPECT_EQ(result, "mock");
}

TEST(DiskManager, TestDelete) {
    auto m = std::make_unique<DiskManager>("file1.db");

    m->Write(1, "foo");
    m->Write(32, "bar");

    m->Delete(32);

    auto result = m->Read(32);
    EXPECT_EQ(result, nullptr);
}
