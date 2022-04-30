global long_mode_start
global load_idt
extern initMultiboot
global refresh_tlb
;extern GDT.Pointer
extern stack_top

%define KERNEL_START 0xC0000000

section .text
extern kernel_main
bits 64

long_mode_start:
    mov rsp, stack_top
    lgdt [init_gdt64_ptr]
    call load_multiboot
    call kernel_main


load_multiboot:
    mov rdi, rax
    mov rsi, rbx
    call initMultiboot
    ret


.data
gdt64:
	dq 0x0000000000000000 ; 0x00 NULL
	dq 0x0020980000000000 ; 0x08 KCODE64
gdt64_end:

init_gdt64_ptr:
	dw gdt64_end - gdt64 - 1
	dq gdt64 - KERNEL_START
