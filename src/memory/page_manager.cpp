#include "page_manager.h"

extern "C" void refresh_tlb();

extern u64 page_l4;

void PageManager::mapPhisicalAddress(u64 physicalAddress, u64 virtualAddress, u64 sizeInBytes) {

    unsigned long p4Index = (virtualAddress >> 39) & 0x1FF;
    unsigned long p3Index = (virtualAddress >> 30) & 0x1FF;
    unsigned long p2Index = (virtualAddress >> 12) & 0x1FFFFF;


    auto *pl3 = (unsigned long*)(((&page_l4)[p4Index] & ~0xFFF) + KERNEL_START);
    auto *pl2 = (unsigned long*)((pl3[p3Index] & ~0xFFF) + KERNEL_START);

    for (unsigned long i = p2Index; i < sizeInBytes / PAGE_SIZE; ++i){
        pl2[i] = physicalAddress | 0x83;
        physicalAddress += PAGE_SIZE;
    }

    refresh_tlb();
}