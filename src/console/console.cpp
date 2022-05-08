#include "../vga.h"
#include "console.h"

using namespace Console;

extern u8 _binary_res_zap_ext_light20_psf_start;
extern u8 _binary_res_zap_ext_light20_psf_end;




auto *header = reinterpret_cast<PSF_font *>(&_binary_res_zap_ext_light20_psf_start);

static u64 xPosition = 0;
static u64 yPosition = 0;

Color currentForeground = Color::BLACK;
Color currentBackground = Color::WHITE;


void putCursor(){

    u32 xOffset = xPosition * header->width;
    u32 yOffset = yPosition * header->height;

    for (u32 i = 0; i < 2; ++i){
        for (int j = 0; j < header->height; ++j){
            drawPixel(xOffset + i, yOffset + j, currentForeground);
        }
    }
}

void fillEmpty(){
    u32 xOffset = xPosition * header->width;
    u32 yOffset = yPosition * header->height;
    for (u32 i = 0; i < header->width; ++i){
        for (int j = 0; j < header->height; ++j){
            drawPixel(xOffset + i, yOffset + j, currentBackground);
        }
    }
}

void printNewLine(){
    fillEmpty();
    xPosition = 0;
    ++yPosition;
    putCursor();
}


inline void Console::putChar(char ch, bool cursor){

    if (ch == '\n'){
        printNewLine();
        return;
    }

    u32 bytesPerLine = (header->width + 7) / 8;
    u8 *glyph = header->headersize + &_binary_res_zap_ext_light20_psf_start +
                (ch > 0 && ch < header->numglyph ? ch : 0) * header->bytesperglyph;

    u32 xOffset = xPosition * header->width;
    u32 yOffset = yPosition * header->height;

    u32 mask;
    for (u32 j = 0; j < header->height; ++j){

        mask = 1 << (header->width - 1);

        for (u32 i = 0; i < header->width; ++i){
            drawPixel(xOffset + i, yOffset + j, *glyph & mask ? currentForeground : currentBackground);
            mask >>= 1;
        }

        glyph += bytesPerLine;
    }

    if (xPosition >= screenWidth() / header->width - 1){
        yPosition++;
        xPosition = 0;
    }
    else {
        xPosition++;
    }

    if (cursor){
        putCursor();
    }
}

void Console::removeChar() {
    fillEmpty();
    if (xPosition == 0){
        xPosition = screenWidth() / header->width - 2;
        yPosition--;
    }
    else {
        xPosition --;
    }
    putChar(' ', false);
    xPosition--;
    putCursor();
}


inline void Console::putChar(char ch){
    putChar(ch, true);

}

void Console::print(const char *string){
    while (*string != '\0'){
        putChar(*string);
        ++string;
    }
}




void clear(){

}


void Console::setTextColor(Color foreground, Color background){
    currentForeground = foreground;
    currentBackground = background;
}


