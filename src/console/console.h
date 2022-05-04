#ifndef CONSOLE_H
#define CONSOLE_H
#include "../types.h"
#include "../vga.h"



namespace Console {

    struct PSF_font {
        u32 magic;
        u32 version;
        u32 headersize;
        u32 flags;
        u32 numglyph;
        u32 bytesperglyph;
        u32 height;
        u32 width;
    }__attribute__((__packed__));


    void print(const char *string);
    void setTextColor(Color foreground, Color background);
    void clear();
    void putChar(char ch);
    void putChar(char ch, bool cursor);
    void removeChar();
}

#endif //CONSOLE_H
