
#pragma once

class Interface {
    public:
        Interface() = default;
        ~Interface() = default;

        /*
         * DeletePage remove a page on disk or in the buffer
         * @param pid is the id of the page
         * */
        virtual void DeletePage(const int pid);

        /*
         * ReadPage read a page on disk or in the buffer pool
         * @param pid is the id of the page
         * TODO-> @return pointer to the page or frame
         * */
        virtual void ReadPage(const int pid);

        /*
         * WritePage write a page on disk or in the buffer pool
         * @param pid is the id of the page
         * @param data contains a buffer with the data to write
         * */
        virtual void WritePage(const int pid, const char *data);
};
