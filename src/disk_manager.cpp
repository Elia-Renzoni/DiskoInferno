
#include <cstring>
#include <mutex>

#include "disk_manager.hpp"
#include "page.hpp"

void DiskManager::Write(const int pid, const char *data) {
    if (pid < 0) return;

    std::unique_lock<std::shared_mutex> lock(DiskManager::latch_);

    DiskPage page;
    int64_t currentOffset = DiskManager::file_.tellp();

    std::memcpy(page.data, data, DATA_REGION);
    page.h.dataSize = DATA_REGION; // 4KB - 21bytes (header)
    page.h.pageID = pid;
    page.h.op = DiskManager::Operation::WRITE;
    page.h.nextOffset = currentOffset + sizeof(page.h) + DATA_REGION;

    if (!DiskManager::offsetLookup_.count(pid)) {
        DiskManager::offsetLookup_[pid] = currentOffset;
    }

    DiskManager::file_.write(
            reinterpret_cast<const char *>(&page.h),
            sizeof(page.h)
    );

    DiskManager::file_.write(page.data, DATA_REGION);
    DiskManager::file_.flush();
}

char* DiskManager::Read(const int pid) {
    if (pid < 0) return nullptr;

    std::shared_lock<std::shared_mutex> lock(DiskManager::latch_);

    int64_t offset = 0;
    DiskPage p;
    if (DiskManager::offsetLookup_.count(pid)) {
        offset = DiskManager::offsetLookup_[pid];
    }

    if (offset != 0) {
        DiskManager::file_.seekg(offset);
        DiskManager::file_.read(reinterpret_cast<char*>(&p.h), 21);
        DiskManager::file_.read(p.data, p.h.dataSize);
        return p.data;
    }

    // if no offset is provided execute a full scan and 
    // rebuild the lookup map if needed
    DiskManager::scan(&p, pid);
    return p.data;
}

void DiskManager::Delete(const int pid) {
    if (pid < 0) return;

    std::unique_lock<std::shared_mutex> lock(DiskManager::latch_);
    int64_t offset = 0;
    DiskPage p;
    if (!DiskManager::offsetLookup_.count(pid)) return;

    offset = DiskManager::offsetLookup_[pid];
    DiskManager::file_.seekg(offset);
    DiskManager::file_.read(reinterpret_cast<char*>(&p.h), 21);
    p.h.op = DiskManager::Operation::DELETE;
    int64_t current = DiskManager::file_.tellp();
    p.h.nextOffset = current + sizeof(p.h) + sizeof(p.data);

    DiskManager::file_.seekp(0, std::ios::end);
    DiskManager::file_.write(reinterpret_cast<char*>(&p.h), 21);
    DiskManager::file_.write(p.data, DATA_REGION);

    DiskManager::file_.flush();

    DiskManager::offsetLookup_.erase(pid);
}


