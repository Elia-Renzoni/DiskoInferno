
#pragma once

class Replacer {
    public:
        Replacer();
        ~Replacer() = default;

    /**
     * Evict returns the best victim choice based on the algorithm used
     * @return page id of the victim.
     */
    virtual int Evict() = 0;

    /**
     * @param {pid} page id
     */
    virtual void Delete(int pid) = 0;

    /**
     * @param {pid} page id
     */
    virtual void Insert(int pid) = 0;


    /**
     * @return size of the buffer used
     */
    virtual int Size() = 0;

};
