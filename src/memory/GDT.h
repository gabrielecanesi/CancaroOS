#ifndef GDT_H
#define GDT_H

typedef unsigned short uint16_t;
typedef unsigned long uint64_t;
typedef unsigned char uint8_t;

struct GdtDescriptor {
    uint16_t size;
    uint64_t offset;
} __attribute((packed));


struct GdtEntry{
    uint16_t limit0;
    uint16_t base0;
    uint16_t base1;
    uint8_t accessByte;
    uint8_t limit1Flags;
    uint8_t base2;
} __attribute((packed));

struct GDT {
    GdtEntry null;
    GdtEntry kernelCode;
    GdtEntry kernelData;
    GdtEntry userNull;
    GdtEntry userCode;
    GdtEntry userData;
}__attribute((packed)) __attribute((aligned(0x1000)));

extern GDT defaultGdt;

extern "C" void load_gdt(GdtDescriptor *gdtGescriptor);

#endif