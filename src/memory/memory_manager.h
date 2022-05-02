#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "../multiboot/multiboot.h"
#include "../types.h"
#include "BitMap.h"

namespace MemoryManager{


    u64 getAvailableRam();
    void initMemory(multiboot_tag_mmap * memory_map);
}

#endif //MEMORY_MANAGER_H
