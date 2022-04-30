#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICw4 0x01
#define ICW4_8086 0x01


struct interrupt_frame;

__attribute((interrupt)) void faultHandler(interrupt_frame *frame);
__attribute((interrupt)) void keyboardHandler(interrupt_frame * frame);
__attribute((interrupt)) void divisionByZeroHandler(interrupt_frame * frame);

void endPICMaster();
void endPICSlave();
void remapPIC();
#endif