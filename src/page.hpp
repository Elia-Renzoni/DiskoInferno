
#include <atomic>
#include <shared_mutex>
#include <cstring>

#pragma once

constexpr int DATA_REGION = 4075; // page size 4096 (4KB) - 21 header size (21 bytes) = 4075

struct Page {
    char data_[DATA_REGION];
    int pageID_;
    std::atomic<int> pins_;
    std::atomic<bool> isDirty_;
    std::shared_mutex latch;

    void pinPage() {
        auto pinCounter = pins_.load();
        pinCounter += 1;
        pins_.store(pinCounter);
    };

    void unpinPage() {
        auto pinCounter = pins_.load();
        if (pinCounter <= 0) return;

        pinCounter -= 1;
        pins_.store(pinCounter);
    };

    bool isPageEvictable() {
        return pins_.load() == 0;
    };

    void addData(char *data) {
        std::memcpy(data_, data, DATA_REGION);
    };

    void addPageID(int pid) {
        pageID_ = pid;
    };

    char *getData() {
        return data_;
    }

    int getPID() {
        return pageID_;
    }
};
