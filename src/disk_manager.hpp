#include <fstream>
#include <unordered_map>
#include <shared_mutex>

#include "interface.hpp"
#include "page.hpp"

#pragma once

class DiskManager : public Interface {
    public:
        DiskManager(const std::string &filePath) {
                    file_.open(filePath, std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
        };
        ~DiskManager() = default;

        enum Operation {
            WRITE, READ, DELETE,
        };

        struct DiskPage {
            struct Header {
                int32_t pageID;
                Operation op;
                int32_t dataSize;
                int64_t nextOffset;
            };

            Header h;
            char data[DATA_REGION];
        };

        void Write(const int pid, const char *data) override;
        std::optional<std::string> Read(const int pid) override;
        void Delete(const int pid) override;
    private:
        void scan(DiskPage *p, int pid) {
            auto len = offsetLookup_.size();
            file_.seekg(0);

            std::istream *result = &file_;
            while (*result) {
                result = &file_.read(reinterpret_cast<char*>(&p->h), sizeof(p->h));
                if (p->h.pageID == pid && p->h.op != DELETE) {
                    char data[p->h.dataSize];
                    file_.read(data, p->h.dataSize);
                    return;
                }

                if (len && p->h.op != DELETE) offsetLookup_[pid] = p->h.nextOffset;
                file_.seekg(p->h.nextOffset);
            }
        };

        std::fstream file_;
        std::unordered_map<int, int64_t> offsetLookup_;
        std::shared_mutex latch_;
};
