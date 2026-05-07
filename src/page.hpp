
#include <atomic>
#include <shared_mutex>


#pragma once

struct Page {
    char *data_;
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
        data_ = data;
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
