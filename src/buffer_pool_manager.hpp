
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
        int generateFrameID();
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
            auto spots = freeSlots.load();
            if (spots <= 0) return;
            spots -= 1;
            freeSlots.store(spots);
        }
        
        void addIndex(char *data, int pid) {
            std::scoped_lock<std::mutex> lock(indexLatch_);

            if (lookupIndex_.count(data)) {
                return;
            }

            lookupIndex_[data] = pid;
        }

        void destroyIndex(char *data) {
            std::scoped_lock<std::mutex> lock(indexLatch_);

            if (lookupIndex_.count(data)) {
                lookupIndex_.erase(data);
            }
        }

        int getIndex(char *data) {
            std::scoped_lock<std::mutex> lock(indexLatch_);

            return lookupIndex_[data];
        }

        void insertRoute(int pageID, int frameID) {
            std::scoped_lock<std::mutex> lock(indexLatch_);

            if (routingMap_.count(frameID)) {
                auto pid = routingMap_[frameID];
                if (pid == pageID) return;
            }

            routingMap_[frameID] = pageID;
        }

        void destroyRoute(int frameID) {
            std::scoped_lock<std::mutex> lock(indexLatch_);

            if (!routingMap_.count(frameID)) return;

            routingMap_.erase(frameID);
        }

        std::optional<int> getRoute(int pageID) {
            std::scoped_lock<std::mutex> lock(indexLatch_);

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

        std::mutex indexLatch_;
        std::unordered_map<char*, int> lookupIndex_;    // TODO-> replace with B+tree
        std::unordered_map<int, int> routingMap_;
};
