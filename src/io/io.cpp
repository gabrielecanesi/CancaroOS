#include "io.h"

void out(unsigned short port, unsigned char value){
    asm volatile("out %0, %1" : : "a"(value), "Nd"(port));
}


unsigned char in(unsigned short port){
    unsigned char result;
    asm volatile ("inb %1, %0"
    : "=a"(result)
    : "Nd"(port));
    return result;
}


void wait(){
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}