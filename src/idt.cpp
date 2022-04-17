#include "idt.h"
#include "interrupts.h"
#include "kernel.h"
#include "io.h"

extern "C" void load_idt();

unsigned char TYPE_INTERRUPT = 0b10001110;
unsigned char TYPE_CALL_GATE = 0b10001100;
unsigned char TRAP_GATE =      0b10001111;

void InterruptDescriptor::setOffset(unsigned long offset){
    offset1 = (unsigned short)(offset &  0x000000000000ffff);
    offset2 = (unsigned short)((offset & 0x00000000ffff0000) >> 16);
    offset3 = (unsigned int)((offset &   0xffffffff00000000) >> 32);
}


unsigned long InterruptDescriptor::getOffset(){
    return (unsigned long)offset1 | (unsigned long)(offset2 << 16) |
    (unsigned long)(offset3 << 32);
}


IDTR idtable;

void setupInterrupts(){
    idtable.limit = 0xFFFF;
    idtable.offset = (unsigned long) 0x143;

    InterruptDescriptor *interrupt = (InterruptDescriptor*)(idtable.offset + 0xE * sizeof(InterruptDescriptor));
    interrupt->setOffset((unsigned long) faultHandler);
    interrupt->type = TYPE_INTERRUPT;
    interrupt->segment_selector =  0x08;


    InterruptDescriptor *keyboard = (InterruptDescriptor*)(idtable.offset + 0x01 * sizeof(InterruptDescriptor));
    keyboard->setOffset((unsigned long) keyboardHandler);
    keyboard->type = TYPE_INTERRUPT;
    keyboard->segment_selector = 0x08;

    InterruptDescriptor *aaaaa = (InterruptDescriptor*)(idtable.offset + 0x00 * sizeof(InterruptDescriptor));
    aaaaa->setOffset((unsigned long) divisionByZeroHandler);
    aaaaa->type = TYPE_INTERRUPT;
    aaaaa->segment_selector = 0x08;

    InterruptDescriptor *p =  (InterruptDescriptor*)(idtable.offset + 0x21 * sizeof(InterruptDescriptor));
    p->setOffset((unsigned long) keyboardHandler);
    p->type = TYPE_INTERRUPT;
    p->segment_selector = 0x08;

    load_idt();

    remapPIC();
    out(PIC1_DATA, 0b11111101);
    out(PIC2_DATA, 0b11111111);

    asm("sti");
    while (true){
    }
    
}