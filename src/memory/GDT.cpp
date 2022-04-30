#include "GDT.h"

__attribute((aligned(0x1000)))
GDT defaultGdt = {
    {0, 0, 0, 0x00, 0x00, 0},
    {0, 0, 0, 0x9a, 0xa0, 0},
    {0, 0, 0, 0x92, 0xa0, 0},
    {0, 0, 0, 0x00, 0x00, 0},
    {0, 0, 0, 0x9a, 0xa0, 0},
    {0, 0, 0, 0x92, 0xa0, 0},
};


/*
extern void initGdt(){
    GdtDescriptor descriptor;
    descriptor.size = sizeof(GDT) - 1;
    descriptor.offset = (unsigned long) &defaultGdt;
    load_gdt(&descriptor);
}*/