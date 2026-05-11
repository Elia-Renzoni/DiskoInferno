
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
     * @param {fid} frame id
     */
    virtual void Delete(int fid) = 0;

    /**
     * @param {fid} frame id
     */
    virtual void Insert(int fid) = 0;


    /**
     * @return size of the buffer used
     */
    virtual int Size() = 0;

};
