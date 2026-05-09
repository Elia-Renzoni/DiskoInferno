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
            p->h.pageID = -1;
            file_.seekg(0);

            while (file_.read(reinterpret_cast<char*>(&p->h), sizeof(p->h))) {
                if (p->h.pageID == pid) {
                    if (p->h.op == DELETE) {
                        p->h.pageID = -1;  //
                        file_.seekg(p->h.nextOffset);
                        continue;
                    }
                    file_.read(p->data, p->h.dataSize);
                    file_.seekg(p->h.nextOffset);
                    continue;
                }
                file_.seekg(p->h.nextOffset);
            }
        };

        std::fstream file_;
        std::unordered_map<int, int64_t> offsetLookup_;
        std::shared_mutex latch_;
};
