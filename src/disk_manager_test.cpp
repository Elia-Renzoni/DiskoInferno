
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
    EXPECT_EQ(result.value(), "qux");

    result = m->Read(22);
    EXPECT_EQ(result.value(), "bar");

    result = m->Read(33);
    EXPECT_EQ(result.value(), "mock");
}

TEST(DiskManager, TestDelete) {
    auto m = std::make_unique<DiskManager>("file1.db");

    m->Write(1, "foo");
    m->Write(32, "bar");

    m->Delete(32);

    auto result = m->Read(32);
    EXPECT_FALSE(result.has_value());
}

TEST(DiskManager, TestUpdate) {
    auto m = std::make_unique<DiskManager>("file2.db");

    m->Write(1, "foo");
    m->Write(32, "bar");
    m->Write(23, "mock");
    m->Write(32, "bar-updated");

    auto result = m->Read(32);
    EXPECT_EQ(result.value(), "bar-updated");
}
