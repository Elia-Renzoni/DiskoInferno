
#include <string>
#include <optional>

#pragma once

class Interface {
    public:
        Interface() = default;
        ~Interface() = default;

        /*
         * Delete remove a page/frame on disk or in the buffer
         * @param pid is the id of the page
         * */
        virtual void Delete(const int pid) = 0;

        /*
         * Read read a page/frame on disk or in the buffer pool
         * @param pid is the id of the page
         * TODO-> @return pointer to the data
         * */
        virtual std::optional<std::string> Read(const int pid) = 0;

        /*
         * Write write a page/frame on disk or in the buffer pool
         * @param pid is the id of the page
         * @param data contains a buffer with the data to write
         * */
        virtual void Write(const int pid, const char *data) = 0;
};
