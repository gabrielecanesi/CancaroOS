#ifndef IDT_H
#define IDT_H


struct InterruptDescriptor{
    unsigned short offset1;
    unsigned short segment_selector;
    unsigned char ist;
    unsigned char type;
    unsigned short offset2;
    unsigned int offset3;
    unsigned int reserved;

    void setOffset(unsigned long offset);
    unsigned long getOffset();
}__attribute((packed));


struct IDTR{
    unsigned short limit;
    unsigned long offset;
}__attribute((packed));


void setupInterrupts();

#endif