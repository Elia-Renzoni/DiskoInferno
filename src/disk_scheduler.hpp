
#include <vector>
#include <shared_mutex>
#include <atomic>
#include <optional>

#include "disk_manager.hpp"

#pragma once 

struct Request {
    enum RequestType {
        READ_ONLY,
        WRITE_ONLY,
        DELETE_ONLY,
    };

    char *data;
    int datalen;
    RequestType operation;
    int pageID;
};

class Scheduler {
    public:
        Scheduler(DiskManager m): diskController(m) {};
        ~Scheduler() = default;

        void addRequest(Request *req);
        void execScheduler(); 
        void dropScheduler();
        void doSchedule();

        std::shared_mutex lock;
        DiskManager diskController;

    private:
        std::optional<Request*> getNext() {
            std::optional<Request*> req;
            if (queueLength.load() > 0) {
                req = diskRequests.front();
                return req;
            } 

            return std::nullopt;
        };

        std::atomic<bool> running;
        std::atomic<int> queueLength;
        std::vector<Request*> diskRequests;
};
