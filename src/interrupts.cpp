#include "interrupts.h"
#include "kernel.h"
#include "io.h"
#include "idt.h"
#include "io.h"
#include "keyboard.h"

__attribute((interrupt)) void faultHandler(interrupt_frame *frame){
    print("Diostronzo");
    while (true);
    
}


__attribute((interrupt)) void keyboardHandler(interrupt_frame *frame){
    unsigned char scanCode = in(0x60);
    handleKeypress(scanCode);
    endPICMaster();
}

__attribute((interrupt)) void divisionByZeroHandler(interrupt_frame *frame){
    print("Divisione per 0 bro");
    while(true);
}

void endPICMaster(){
    out(PIC1_COMMAND, PIC_EOI);
}


void endPICSlave(){
    out(PIC2_COMMAND, PIC_EOI);
    out(PIC1_COMMAND, PIC_EOI);
}

void remapPIC(){
    unsigned char a1, a2;

    a1 = in(PIC1_DATA);
    wait();
    a2 = in(PIC2_DATA);
    wait();

    out(PIC1_COMMAND, ICW1_INIT | ICW1_ICw4);
    wait();

    out(PIC2_COMMAND, ICW1_INIT | ICW1_ICw4);


    out(PIC1_DATA, 0x20);
    wait();
    out(PIC2_DATA, 0x28);
    wait();


    out(PIC1_DATA, 4);
    wait();
    out(PIC2_DATA, 2);
    wait();


    out(PIC1_DATA, ICW4_8086);
    wait();
    out(PIC2_DATA, ICW4_8086);
    wait();

    out(PIC1_DATA, a1);
    wait();
    out(PIC2_DATA, a2);

}