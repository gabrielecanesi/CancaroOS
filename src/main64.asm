global long_mode_start
extern kernel_main
extern idtable
global load_idt



section .text
bits 64

long_mode_start:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call kernel_main
    hlt

load_idt:
    lidt [idtable]
    ret