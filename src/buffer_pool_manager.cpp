
#include <string>

#include "buffer_pool_manager.hpp"


void BufferPoolManager::bootBufferPoolManager() {
    auto maxBlocks = BufferPoolManager::maxFrames;
    for (int i = 0; i < maxBlocks; i++) {
        auto emptyFrame = Frame();
        BufferPoolManager::bufferPool.push_back(emptyFrame);
    }

    BufferPoolManager::freeSlots.store(maxBlocks);
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

std::optional<std::string> BufferPoolManager::Read(const int fid) {
    return std::nullopt;
}
