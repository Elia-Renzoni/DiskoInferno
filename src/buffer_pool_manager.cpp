#include <string>

#include "buffer_pool_manager.hpp"

void BufferPoolManager::bootBufferPoolManager() {
    auto maxBlocks = BufferPoolManager::maxFrames;
    for (int i = 0; i < maxBlocks; i++) {
        auto emptyFrame = Frame();
        BufferPoolManager::bufferPool.push_back(emptyFrame);
    }

    BufferPoolManager::freeSlots.store(maxBlocks);
    BufferPoolManager::monotonicFrameID.store(0);
}

std::optional<int> BufferPoolManager::generateFrameID() {
    auto latestFrameID = BufferPoolManager::monotonicFrameID.load();
    while (true) {
        if (latestFrameID >= BufferPoolManager::maxFrames)
            return std::nullopt;

        if (BufferPoolManager::monotonicFrameID.compare_exchange_weak(latestFrameID, latestFrameID + 1))
            return latestFrameID + 1;
    }
}

void BufferPoolManager::Write(const int fid, const char *data) {
    /*
    auto result = BufferPoolManager::findPage(pid);
    Page *page;
    if (result.has_value()) {
        page = result.value();
    }

    page->pinPage();
    std::unique_lock lock(page->latch);
    // TODO-> replacing policy and write operation
    //
    // for eviction:


    page->unpinPage();
    */
}

void BufferPoolManager::Delete(const int fid) {
}

std::optional<std::string> BufferPoolManager::Read(const int pid) {
    return std::nullopt;
}

std::optional<int> BufferPoolManager::findPageID(char *data) {
    return BufferPoolManager::getIndex(data);
}

std::optional<int> BufferPoolManager::findFrameID(int pid) {
    return BufferPoolManager::getRoute(pid);
}

