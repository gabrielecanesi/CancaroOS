#include "vga.h"
#include "multiboot/multiboot.h"


multiboot_tag_framebuffer *framebufferInfo;

void drawPixel(u64 x, u64 y, Color color){
    auto *pixel = reinterpret_cast<u32*>(fb_addr + x * 4 + y * framebufferInfo->common.framebuffer_pitch);
    *pixel = static_cast<u32>(color);
}


void initVideo(multiboot_tag_framebuffer *framebufferTag){
    framebufferInfo = framebufferTag;
}

u32 screenWidth(){
    return framebufferInfo->common.framebuffer_width;
}