
#include <vector>
#include <optional>
#include <shared_mutex>
#include <atomic>

#include "interface.hpp"
#include "frame.hpp"

#pragma once

class BufferPoolManager : public Interface {
    public:
        BufferPoolManager(const int maxFrames): maxFrames(maxFrames) {};
        ~BufferPoolManager() = default;
       
        void bootBufferPoolManager();
        void Write(const int fid, const char *data) override;
        std::optional<std::string> Read(const int fid) override;
        void Delete(const int fid) override;
    private:
        
        std::optional<Frame*> findPage(int frameToSearch) {
            std::optional<Frame*> block;
            for (auto& frame : bufferPool) {
                auto frameId = frame.getFID();
                if (frameId == frameToSearch) {
                    block = &frame;
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

        const int maxFrames;
        std::vector<Frame> bufferPool;
        std::atomic<int> freeSlots;
        std::shared_mutex bufferPoolLock;
};
