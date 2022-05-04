#ifndef VGA_H
#define VGA_H
#include "types.h"
#include "multiboot/multiboot.h"



constexpr u64 fb_addr = 0xFFFFFF0000000000;

enum class Color : u32 {
    BLACK = 0,
    WHITE = 0xFFFFFFFF,
    LIGHT_BLUE = 0x9db5c9
};


void drawPixel(u64 x, u64 y, Color color);
void initVideo(multiboot_tag_framebuffer *framebufferTag);
u32 screenWidth();

#endif //VGA_H
