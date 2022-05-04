#include "idt.h"
#include "interrupts.h"
#include "../io/io.h"
#include "../memory/page_manager.h"

extern "C" void load_idt();

u8 TYPE_INTERRUPT = 0b10001110;
u8 TYPE_CALL_GATE = 0b10001100;
u8 TRAP_GATE =      0b10001111;

void InterruptDescriptor::setOffset(u64 offset){
    offset1 = (u16)(offset &  0x000000000000ffff);
    offset2 = (u16)((offset & 0x00000000ffff0000) >> 16);
    offset3 = (u32)((offset &   0xffffffff00000000) >> 32);
}


u64 InterruptDescriptor::getOffset(){
    return (u64)offset1 | (u64)(offset2 << 16) |
    (u64)(offset3 << 32);
}


IDTR idtable;

InterruptDescriptor interruptDescriptors[10];

void setupInterrupts(){
    idtable.limit = 0xFFFF;
    idtable.offset = reinterpret_cast<u64>(&interruptDescriptors);

    auto *interrupt = (InterruptDescriptor*)(idtable.offset + 0xE * sizeof(InterruptDescriptor));
    interrupt->setOffset((u64) faultHandler);
    interrupt->type = TYPE_INTERRUPT;
    interrupt->segment_selector =  0x08;


    auto *keyboard = (InterruptDescriptor*)(idtable.offset + 0x01 * sizeof(InterruptDescriptor));
    keyboard->setOffset((u64) keyboardHandler);
    keyboard->type = TYPE_INTERRUPT;
    keyboard->segment_selector = 0x08;

    auto *aaaaa = (InterruptDescriptor*)(idtable.offset + 0x00 * sizeof(InterruptDescriptor));
    aaaaa->setOffset((u64) divisionByZeroHandler);
    aaaaa->type = TYPE_INTERRUPT;
    aaaaa->segment_selector = 0x08;

    auto *p =  (InterruptDescriptor*)(idtable.offset + 0x21 * sizeof(InterruptDescriptor));
    p->setOffset((u64) keyboardHandler);
    p->type = TYPE_INTERRUPT;
    p->segment_selector = 0x08;

    load_idt();

    remapPIC();
    out(PIC1_DATA, 0b11111101);
    out(PIC2_DATA, 0b11111111);

    asm("sti");
}