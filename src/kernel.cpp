#include "kernel.h"
#include "idt.h"
extern "C" void kernel_main();

struct Char{
    char value;
    unsigned char color;

    Char(const char value, unsigned char foreground, unsigned char background){
        this->value = value;
        this->color = foreground | background << 4;
    }
};


Char* BASE_PRINT_ADDRESS = (Char*) 0xb8000;
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

void kernel_main(){
    clear();
    set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    const char* porcoddio = "";
    print(porcoddio);
    setupInterrupts();
}

void print(long number){
    if (number < 0){
        print("-");
        print(-number);
    } else if (number < 10) {
        const char n[] = {number + '0'};
        print(n);
    } else {
        long remainder = number % 10;
        long divided = number / 10;

        const char n[] = {remainder + '0'};
        print(divided);
        print(n);
    }
}
