
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
            while (Scheduler::running.load()) {
                Scheduler::doSchedule();
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
            Scheduler::diskController.Write(elem->pageID, elem->data);
        case Request::DELETE_ONLY:
            Scheduler::diskController.Delete(elem->pageID);
    }
}
