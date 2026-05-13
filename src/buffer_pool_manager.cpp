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

int BufferPoolManager::generateFrameID() {
    auto latestFrameID = BufferPoolManager::monotonicFrameID.load();
    if (latestFrameID >= BufferPoolManager::maxFrames) {
        return -1;
    }

    ++latestFrameID;
    BufferPoolManager::monotonicFrameID.store(latestFrameID);
    return latestFrameID;
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

