#include "BitMap.h"

u64 BitMap::size() {
    return m_size;
}

bool BitMap::operator[](u64 index) {
    return (m_buffer[index / 8] >> (7 - index % 8)) & 1;
}


void BitMap::set(u64 index, bool status) {
    u8 mask = status << (7 - (index % 8));

    if (status){
        m_buffer[index / 8] |= mask;
    }
    else {
        m_buffer[index / 8] &= ~mask;
    }
}


void BitMap::init(u64 size, u8* address) {
    m_size = size;
    m_buffer = address;
    for (u64 i = 0; i < size; ++i){
        m_buffer[i] = 0;
    }
}
