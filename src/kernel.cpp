#include "multiboot/multiboot.h"
#include "types.h"
#include "memory/page_manager.h"
#include "vga.h"
#include "memory/memory_manager.h"



extern "C" void initMultiboot(u64 magic, unsigned long addr){

    addr += PageManager::KERNEL_START;

    struct multiboot_tag *tag;
    unsigned size;

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        return;
    }

    if (addr & 7) {
        return;
    }


    size = *(unsigned *) addr;

    for (tag = (struct multiboot_tag *) (addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag *) ((u8 *) tag
                                         + ((tag->size + 7) & ~7))) {

        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_MMAP:
                MemoryManager::initMemory(reinterpret_cast<multiboot_tag_mmap *>(tag));
                multiboot_memory_map_t *mmap;
                u64 availableRam;
                availableRam = 0;
                for (mmap = ((multiboot_tag_mmap *) tag)->entries;
                (u8 *) mmap < (u8 *) tag + tag->size;
                mmap = (multiboot_memory_map_t *) ((u64) mmap + ((multiboot_tag_mmap *) tag)->entry_size)) {
                    if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE){
                        availableRam += mmap->len;
                    }
                }

                break;

            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
                u32 color;
                unsigned i;
                auto *tagfb = (multiboot_tag_framebuffer *) tag;
                void *fb = (void *) (u64) tagfb->common.framebuffer_addr;



                switch (tagfb->common.framebuffer_type) {

                    case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED: {
                        unsigned best_distance, distance;
                        struct multiboot_color *palette;

                        palette = tagfb->framebuffer_palette;

                        color = 0;
                        best_distance = 4*256*256;

                        for (i = 0; i < tagfb->framebuffer_palette_num_colors; i++) {
                            distance = (0xff - palette[i].blue)
                                       * (0xff - palette[i].blue)
                                       + palette[i].red * palette[i].red
                                       + palette[i].green * palette[i].green;
                            if (distance < best_distance) {
                                color = i;
                                best_distance = distance;
                            }
                        }
                    }
                        break;

                    case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
                        color = (((1 << tagfb->framebuffer_blue_mask_size) - 1) << tagfb->framebuffer_blue_field_position) |
                                (((1 << tagfb->framebuffer_green_mask_size) - 1) << tagfb->framebuffer_green_field_position);
                        break;

                    case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
                        color = '\\' | 0x0110;
                        break;

                    default:
                        color = 0xfff00fff;
                        break;
                }

                PageManager::mapPhisicalAddress((u64)fb, fb_addr, 1024 * 1024 * 16);



                for (i = 0; i < tagfb->common.framebuffer_width; i++) {
                    for (int j = 0; j < tagfb->common.framebuffer_height; ++j){

                        switch (tagfb->common.framebuffer_bpp) {
                            case 8: {
                                u8 *pixel = (u8 *) fb_addr + tagfb->common.framebuffer_pitch * i + i;
                                *pixel = color;
                            }
                                break;
                            case 15:
                            case 16: {
                                auto *pixel = (u16 *)((u64)fb_addr + (u64)(tagfb->common.framebuffer_pitch * i + 2 * i));
                                *pixel = color;
                            }
                                break;
                            case 24: {
                                auto *pixel = (u32 *)((u64)fb_addr + (u64)(i * 3 + tagfb->common.framebuffer_pitch * j));
                                *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
                            }
                                break;

                            case 32: {
                                auto *pixel = (u32 *)((u64)fb_addr + (u64)(i * 4 + tagfb->common.framebuffer_pitch * j));
                                *pixel = color;
                            }
                                break;
                        }
                    }
                }
                break;
            }
        }
    }
    tag = (multiboot_tag *) ((u8 *) tag
                                    + ((tag->size + 7) & ~7));
}

extern "C" void kernel_main(u64 multibootHeaderAddress){
    while (true);
    //setupInterrupts();
}
