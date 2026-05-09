
#include <cstring>
#include <mutex>
#include <optional>

#include "disk_manager.hpp"
#include "page.hpp"

void DiskManager::Write(const int pid, const char *data) {
    if (pid < 0) return;

    std::unique_lock<std::shared_mutex> lock(DiskManager::latch_);

    DiskPage page;
    int64_t currentOffset = DiskManager::file_.tellp();

    page.h.dataSize = std::strlen(data) + 1;
    page.h.pageID = pid;
    page.h.op = DiskManager::Operation::WRITE;
    page.h.nextOffset = currentOffset + sizeof(page.h) + page.h.dataSize;
    std::memcpy(page.data, data, page.h.dataSize);

    if (!DiskManager::offsetLookup_.count(pid)) {
        DiskManager::offsetLookup_[pid] = currentOffset;
    }

    DiskManager::file_.write(
            reinterpret_cast<const char *>(&page.h),
            sizeof(page.h)
    );

    DiskManager::file_.write(page.data, page.h.dataSize);
    DiskManager::file_.flush();
}

std::optional<std::string> DiskManager::Read(const int pid) {
    if (pid < 0) return std::nullopt;

    std::shared_lock<std::shared_mutex> lock(DiskManager::latch_);

    int64_t offset = -1;
    DiskPage p;
    if (DiskManager::offsetLookup_.count(pid)) {
        offset = DiskManager::offsetLookup_[pid];
    }

    if (offset != -1) {
        DiskManager::file_.seekg(offset);
        DiskManager::file_.read(reinterpret_cast<char*>(&p.h), sizeof(p.h));
        DiskManager::file_.read(p.data, p.h.dataSize);
        return std::string(p.data, p.h.dataSize - 1);
    }

    // if no offset is provided execute a full scan and 
    // rebuild the lookup map if needed
    DiskManager::scan(&p, pid);
    return std::string(p.data, p.h.dataSize - 1);
}

void DiskManager::Delete(const int pid) {
    if (pid < 0) return;

    std::unique_lock<std::shared_mutex> lock(DiskManager::latch_);
    int64_t offset = 0;
    DiskPage p;
    if (!DiskManager::offsetLookup_.count(pid)) return;

    offset = DiskManager::offsetLookup_[pid];
    DiskManager::file_.seekg(offset);
    DiskManager::file_.read(reinterpret_cast<char*>(&p.h), sizeof(p.h));
    p.h.op = DiskManager::Operation::DELETE;
    int64_t current = DiskManager::file_.tellp();
    p.h.nextOffset = current + sizeof(p.h) + sizeof(p.data);

    DiskManager::file_.seekp(0, std::ios::end);
    DiskManager::file_.write(reinterpret_cast<char*>(&p.h), sizeof(p.h));
    DiskManager::file_.write(p.data, DATA_REGION);

    DiskManager::file_.flush();

    DiskManager::offsetLookup_.erase(pid);
}


