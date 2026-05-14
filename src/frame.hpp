
#include <atomic>
#include <shared_mutex>
#include <cstring>

#pragma once

constexpr int DATA_REGION = 4075; 

struct Frame { 
    char data_[DATA_REGION]; 
    int frameID_;
    std::atomic<int> pins_;
    std::atomic<bool> isDirty_;
    std::shared_mutex latch;

    void pinFrame() {
        pins_.fetch_add(1);
    };

    void unpinFrame() {
        auto pinCounter = pins_.load();
        while (true) {
            if (pinCounter <= 0) return;

            if (pins_.compare_exchange_weak(pinCounter, pinCounter - 1)) return;
        }
    };

    bool isFrameEvictable() {
        return pins_.load() == 0;
    };

    void addData(char *data) {
        std::memcpy(data_, data, DATA_REGION);
    };

    void addFrameID(int fid) {
        frameID_ = fid;
    };

    char *getData() {
        return data_;
    }

    int getFID() {
        return frameID_;
    }
};
