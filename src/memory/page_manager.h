#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "../types.h"

namespace PageManager {
    constexpr u64 PAGE_SIZE = 0x200000;
    constexpr u64 KERNEL_START = 0xFFFFFFFF80000000;


    void mapPhisicalAddress(u64 phisicalAddress, u64 virtualAddress, u64 sizeInBytes);

}

#endif
