#include "kernel.h"
#include "interrupt/idt.h"
#include "multiboot/multiboot.h"


unsigned long KERNEL_START = 0xffffffff80000000;
unsigned long fb_addr =      0xFFFFFF0000000000;

extern "C" void refresh_tlb();

extern unsigned long page_l4;
unsigned long *page4;
extern unsigned long page_l2;
extern unsigned long page_l3;
extern unsigned long page_l3_h;
extern unsigned long page_fb;

void mapPage(unsigned long physical, unsigned long virtualAddr){

    unsigned long p4Index = (virtualAddr >> 39) & 0x1FF;
    unsigned long p3Index = (virtualAddr >> 30) & 0x1FF;
    unsigned long p2Index = (virtualAddr >> 12) & 0x1FFFFF;


    auto *pl3 = (unsigned long*)(((&page_l4)[p4Index] & ~0xFFF) + KERNEL_START);
    auto *pl2 = (unsigned long*)((pl3[p3Index] & ~0xFFF) + KERNEL_START);

    for (unsigned long i = p2Index; i < 512; ++i){
        pl2[i] = physical | 0x83;
        physical += 0x200000;
    }
    refresh_tlb();
}

extern "C" void initMultiboot(unsigned long magic, unsigned long addr){
    addr += KERNEL_START;
    //setupPages();
    //clear();
    //set_color(PRINT_COLOR_LIGHT_GRAY, PRINT_COLOR_BLACK);

    struct multiboot_tag *tag;
    unsigned size;

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        print((unsigned long)MULTIBOOT2_BOOTLOADER_MAGIC);
        print("; ");
        print((unsigned long) magic);
        return;
    }

    if (addr & 7) {
        print ((unsigned long) addr);
        return;
    }


    size = *(unsigned *) addr;
    //print ((unsigned long)size);
    //print ("; HEADERS: ");

    for (tag = (struct multiboot_tag *) (addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag
                                         + ((tag->size + 7) & ~7))) {

        /*print(itostr(tag->size));
        print(", type: ");
        print(itostr(tag->type));*/
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                break;
            case MULTIBOOT_TAG_TYPE_MODULE:
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                break;
            case MULTIBOOT_TAG_TYPE_BOOTDEV:
                break;
            case MULTIBOOT_TAG_TYPE_MMAP:
                multiboot_memory_map_t *mmap;

                for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
                (multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag + tag->size;
                     mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + ((struct multiboot_tag_mmap *) tag)->entry_size)) {
                }

                break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
                multiboot_uint32_t color;
                unsigned i;
                struct multiboot_tag_framebuffer *tagfb = (multiboot_tag_framebuffer *) tag;
                void *fb = (void *) (unsigned long long) tagfb->common.framebuffer_addr;



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




                mapPage((unsigned long)fb, fb_addr);
                for (i = 0; i < tagfb->common.framebuffer_width; i++) {
                    for (int j = 0; j < tagfb->common.framebuffer_height; ++j){

                        switch (tagfb->common.framebuffer_bpp) {
                            case 8: {
                                multiboot_uint8_t *pixel = (unsigned char *) fb_addr + tagfb->common.framebuffer_pitch * i + i;
                                *pixel = color;
                            }
                                break;
                            case 15:
                            case 16: {
                                multiboot_uint16_t *pixel = (unsigned short *)((unsigned long)fb_addr + (unsigned long )(tagfb->common.framebuffer_pitch * i + 2 * i));
                                *pixel = color;
                            }
                                break;
                            case 24: {
                                multiboot_uint32_t *pixel = (unsigned int *)((unsigned long)fb_addr + (unsigned long)(i * 3 + tagfb->common.framebuffer_pitch * j));
                                *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
                            }
                                break;

                            case 32: {
                                multiboot_uint32_t *pixel = (unsigned int *)((unsigned long)fb_addr + (unsigned long)(i * 4 + tagfb->common.framebuffer_pitch * j));
                                *pixel = color;
                            }
                                break;
                        }
                    }

                    /*print("     ");
                    print((unsigned long) fb);*/
                }
                break;
            }

        }
        //print("; ");
    }
    tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag
                                    + ((tag->size + 7) & ~7));
}

extern "C" void kernel_main(unsigned long multibootHeaderAddress){
    set_color(PRINT_COLOR_LIGHT_GRAY, PRINT_COLOR_BLACK);
    while (true);
    //setupInterrupts();
}

void print(unsigned long number){
  /*  if (number < 0){
        print("-");
        print(-number);
    } else */if (number < 10) {
        const char n[] = {number + '0'};
        print(n);
    } else {
        long remainder = number % 10;
        long divided = number / 10;

        const char n[] = {remainder + '0'};
        print((unsigned long) divided);
        print(n);
    }
}

char buf[20];
char *itostr(unsigned long number){
    int index = 19;
    buf[19] = 0;
    while (number != 0){
        buf[--index] = (number % 10) + '0';
        number /= 10;
    }
    return &buf[index];
}


struct Char{
    char value;
    unsigned char color;

    Char(const char value, unsigned char foreground, unsigned char background){
        this->value = value;
        this->color = foreground | background << 4;
    }
};


Char* BASE_PRINT_ADDRESS = (Char*) 0xb8000 + KERNEL_START;
int current_char_offset = 0;
unsigned char current_foreground;
unsigned char current_background;

void clear(){
    current_char_offset = 0;
    unsigned char old_foreground = current_foreground;
    unsigned char old_background = current_background;
    set_color(PRINT_COLOR_BLACK, PRINT_COLOR_BLACK);
    for (int i = 0; i < 10000; ++i){
        print(" ");
    }
    current_char_offset = 0;
    set_color(old_foreground, old_background);
}

void set_color(unsigned char foreground, unsigned char background){
    current_background = background;
    current_foreground = foreground;
}



void print(const char *str){
    int i = 0;
    for (; str[i] != '\0'; ++i){
        Char ch(str[i], current_foreground, current_background);
        BASE_PRINT_ADDRESS[i + current_char_offset] = ch;
    }
    current_char_offset += i;
}


void print(const char ascii){
    char x[] = {ascii};
    print(x);
}
char *itostr(unsigned long number);

