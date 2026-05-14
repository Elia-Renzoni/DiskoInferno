
#include <vector>
#include <optional>
#include <shared_mutex>
#include <atomic>
#include <unordered_map>
#include <mutex>

#include "interface.hpp"
#include "frame.hpp"

#pragma once

class BufferPoolManager : public Interface {
    public:
        BufferPoolManager(const int maxFrames): maxFrames(maxFrames) {};
        ~BufferPoolManager() = default;
       
        void bootBufferPoolManager();
        std::optional<int> generateFrameID();
        std::optional<int> findPageID(char *data);
        std::optional<int> findFrameID(int pid);

        void Write(const int fid, const char *data) override;
        std::optional<std::string> Read(const int pid) override;
        void Delete(const int fid) override;
    private:
        [[maybe_unused]]
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
            auto tryDecrease = [&]() -> bool {
                auto spots = freeSlots.load();

                while (true) {
                    if (spots <= 0) return false;

                    if (freeSlots.compare_exchange_weak(spots, spots - 1)) return true;
                }
            };

            tryDecrease();
        }
        
        void addIndex(char *data, int pid) {
            std::unique_lock<std::shared_mutex> lock(indexLatch_);

            if (lookupIndex_.count(data)) {
                return;
            }

            lookupIndex_[data] = pid;
        }

        void destroyIndex(char *data) {
            std::unique_lock<std::shared_mutex> lock(indexLatch_);

            if (lookupIndex_.count(data)) {
                lookupIndex_.erase(data);
            }
        }

        int getIndex(char *data) {
            std::shared_lock<std::shared_mutex> lock(indexLatch_);

            return lookupIndex_[data];
        }

        void insertRoute(int pageID, int frameID) {
            std::unique_lock<std::shared_mutex> lock(routingMapLatch_);

            if (routingMap_.count(frameID)) {
                auto pid = routingMap_[frameID];
                if (pid == pageID) return;
            }

            routingMap_[frameID] = pageID;
        }

        void destroyRoute(int frameID) {
            std::unique_lock<std::shared_mutex> lock(routingMapLatch_);

            if (!routingMap_.count(frameID)) return;

            routingMap_.erase(frameID);
        }

        std::optional<int> getRoute(int pageID) {
            std::shared_lock<std::shared_mutex> lock(routingMapLatch_);

            auto it = routingMap_.begin();
            while (it != routingMap_.end()) {
                if (pageID == it->second) {
                    return it->first;
                }
                it++;
            }

            return std::nullopt;
        } 

        const int maxFrames;
        std::atomic<int> monotonicFrameID;
        std::vector<Frame> bufferPool;
        std::atomic<int> freeSlots;
        std::shared_mutex bufferPoolLock;

        std::shared_mutex indexLatch_;
        std::unordered_map<char*, int> lookupIndex_;    // TODO-> replace with B+tree
        std::shared_mutex routingMapLatch_;
        std::unordered_map<int, int> routingMap_;
};
