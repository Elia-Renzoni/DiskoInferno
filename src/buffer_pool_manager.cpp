
#include "buffer_pool_manager.hpp"


void BufferPoolManager::bootBufferPoolManager() {
    auto maxBlocks = BufferPoolManager::maxPages;
    for (int i = 0; i < maxBlocks; i++) {
        auto emptyPage = Page();
        BufferPoolManager::bufferPool.push_back(emptyPage);
    }

    BufferPoolManager::freeSlots.store(maxBlocks);
}

void BufferPoolManager::Write(const int pid, const char *data) {
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

void BufferPoolManager::Delete(const int pid) {
}

void BufferPoolManager::Read(const int pid) {
}
