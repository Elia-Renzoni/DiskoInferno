
#include <vector>
#include <optional>
#include <shared_mutex>

#include "interface.hpp"
#include "page.hpp"

#pragma once

class BufferPoolManager : public Interface {
    public:
        BufferPoolManager(const int maxPages): maxPages(maxPages) {};
        ~BufferPoolManager() = default;
       
        void bootBufferPoolManager();
        void WritePage(const int pid, const char *data) override;
        void ReadPage(const int pid) override;
        void DeletePage(const int pid) override;
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

        const int maxPages;
        std::vector<Page> bufferPool;
        std::shared_mutex bufferPoolLock;
};
