
#include <mutex>
#include <thread>

#include "disk_scheduler.hpp"

void Scheduler::addRequest(Request *req) {
    std::unique_lock lock(Scheduler::lock);

    int val = Scheduler::queueLength.load();
    val += 1;
    Scheduler::queueLength.store(val);

    Scheduler::diskRequests.push_back(req);
}

void Scheduler::execScheduler() {
    std::thread([this] {
            bool isAlive = Scheduler::running.load();
            while (isAlive) {
                Scheduler::doSchedule();
                isAlive = Scheduler::running.load();
            }
    });
}

void Scheduler::dropScheduler() {
    Scheduler::running.store(false);
}

void Scheduler::doSchedule() {
    std::unique_lock lock(Scheduler::lock);

    auto req = Scheduler::getNext();
    if (!req.has_value()) return;

    auto elem = req.value();
    switch (elem->operation) {
        case Request::READ_ONLY:
            return; // not supported 
        case Request::WRITE_ONLY:
            Scheduler::diskController.WritePage(elem->pageID, elem->data);
        case Request::DELETE_ONLY:
            Scheduler::diskController.DeletePage(elem->pageID);
    }
}
