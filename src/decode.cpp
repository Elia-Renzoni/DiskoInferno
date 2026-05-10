// decode.cpp
#include <fstream>
#include <iostream>
#include <cstdint>

struct Header {
    int32_t pageID;
    int32_t op;
    int32_t dataSize;
    int64_t nextOffset;
};

int main() {
    std::fstream f("file1.db", std::ios::binary | std::ios::in);
    Header h;
    int record = 0;

    while (f.read(reinterpret_cast<char*>(&h), sizeof(h))) {
        char data[h.dataSize];
        f.read(data, h.dataSize);

        std::cout << "Record " << record++ << ":\n";
        std::cout << "  pageID     = " << h.pageID << "\n";
        std::cout << "  op         = " << (h.op == 0 ? "WRITE" : "DELETE") << "\n";
        std::cout << "  dataSize   = " << h.dataSize << "\n";
        std::cout << "  nextOffset = " << h.nextOffset << "\n";
        std::cout << "  data       = " << data << "\n\n";

        f.seekg(h.nextOffset);
    }
}
