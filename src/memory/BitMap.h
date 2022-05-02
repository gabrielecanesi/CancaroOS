#ifndef BITMAP_H
#define BITMAP_H

#include "../types.h"

class BitMap {
public:
    u64 m_size;
    u8 *m_buffer;
    bool operator[](u64 index);
    void set(u64 index, bool status);
    u64 size();
    void init(u64 size, u8* address);
};


#endif //BITMAP_H
