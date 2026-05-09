
#include <vector>
#include <optional>
#include <shared_mutex>
#include <atomic>

#include "interface.hpp"
#include "page.hpp"

#pragma once

class BufferPoolManager : public Interface {
    public:
        BufferPoolManager(const int maxPages): maxPages(maxPages) {};
        ~BufferPoolManager() = default;
       
        void bootBufferPoolManager();
        void Write(const int pid, const char *data) override;
        std::optional<std::string> Read(const int pid) override;
        void Delete(const int pid) override;
    private:
        
        std::optional<Page*> findPage(int pidToSearch) {
            std::optional<Page*> block;
            for (auto& page : bufferPool) {
                auto pid = page.getPID();
                if (pid == pidToSearch) {
                    block = &page;
                }
            }

            return nullptr;
        }

        bool freeSpaceAvailable() {
            return freeSlots.load() > 0;
        }

        void decrease() {
            auto spots = freeSlots.load();
            if (spots <= 0) return;
            spots -= 1;
            freeSlots.store(spots);
        }

        const int maxPages;
        std::vector<Page> bufferPool;
        std::atomic<int> freeSlots;
        std::shared_mutex bufferPoolLock;
};
