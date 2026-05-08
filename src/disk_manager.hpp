
#include "interface.hpp"

#pragma once

class DiskManager : public Interface {
    public:
        DiskManager() {};
        ~DiskManager() = default;

        void Write(const int pid, const char *data) override;
        void Read(const int pid) override;
        void Delete(const int pid) override;
};
