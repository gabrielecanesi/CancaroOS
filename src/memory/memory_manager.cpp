#include "memory_manager.h"
#include "page_manager.h"

static u64 availableRam = 0;
u8 buffer = 0;

BitMap bitMap;
u64 MemoryManager::getAvailableRam() {
    return availableRam;
}


void MemoryManager::initMemory(multiboot_tag_mmap *memory_map) {

    for (auto *mmap = memory_map->entries;
    reinterpret_cast<u64>(mmap) < memory_map->size + reinterpret_cast<u64>(memory_map);
     mmap ++){
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE){
            availableRam += mmap->len;
        }
    }

    u64 mapSize = availableRam / PageManager::PAGE_SIZE + 1;
    bitMap.init(mapSize, &buffer);
}
