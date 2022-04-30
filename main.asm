global start
global gdt64.pointer
global magic
global addr
extern long_mode_start
extern initMultiboot
global page_l4
global page_l3
global page_l2
global pages_l3
global pages_l2
global page_l3_h
extern setupPages
global GDT.Pointer
global stack_top

%define KERNEL_START 0xC0000000


section .text
start:
    mov esp, stack_top - KERNEL_START

bits 32
    ; Push the multiboot data on the stack
    push eax
    push ebx

    call check_multiboot
    call check_cpuid
    call check_long_mode
    ;call set_page_tables
    call enable_paging
    lgdt [GDT.Pointer - KERNEL_START]

    jmp (0x8):(start64 - KERNEL_START)

    bits 64



start64:
    mov ax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov rax, long_mode_start
    jmp rax


bits 32

set_page_tables:

    mov ecx, 0
.loop:
    mov eax, 0x200000
    mul ecx
    or eax, 0b10000011
    mov [page_l2 + ecx * 8 - KERNEL_START], eax

    inc ecx
    cmp ecx, 512
    jne .loop

    ret

enable_paging:
    ; Posizione in memoria della page table
    mov eax, page_l4 - KERNEL_START
    mov cr3, eax

    ; PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax
    
    ; Long mode
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Abilita paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    ret

check_cpuid:
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx
    je .no_cpuid
    ret

.no_cpuid:
    mov al, "C"
    jmp error

check_multiboot:
    cmp eax, 0x36d76289
    jne .no_multiboot
    ret

.no_multiboot:
    mov al, "M"
    jmp error


check_long_mode:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .no_long_mode
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .no_long_mode
    ret

.no_long_mode:
    mov al, "L"

error:
    mov byte [0xb8000], al
    hlt

%macro gen_pd_2mb 3
	%assign i %1
	%rep 2
		dq (i | 0x83)
		%assign i i+0x200000
	%endrep
	%rep %3
		dq 0
	%endrep
%endmacro


section .rodata
align 4096
page_l4:
	; map lower half of memory
	dq page_l3 + 0x3 - KERNEL_START
	times 510 dq 0 ; padding

	; map higher half of memory
	dq page_l3_h + 0x3 - KERNEL_START
page_l3:
	; map lower half of memory
	dq page_l2 + 0x3 - KERNEL_START
	times 511 dq 0 ; padding
page_l3_h:
	times 510 dq 0 ; padding

	; map higher half of memory
	dq page_l2 + 0x3 - KERNEL_START
	dq 0
page_l2:
	; identity map 1 GB
	;gen_pd_2mb 0, 10, 502
	gen_pd_2mb 0, 512, 0

 stack_bottom:
    resb 4096 * 4
 stack_top:


; Access bits
PRESENT        equ 1 << 7
NOT_SYS        equ 1 << 4
EXEC           equ 1 << 3
DC             equ 1 << 2
RW             equ 1 << 1
ACCESSED       equ 1 << 0
 
; Flags bits
GRAN_4K       equ 1 << 7
SZ_32         equ 1 << 6
LONG_MODE     equ 1 << 5
 
GDT:
    .Null: equ $ - GDT
        dq 0
    .Code: equ $ - GDT
        dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
        db 0                                        ; Base (mid, bits 16-23)
        db PRESENT | NOT_SYS | EXEC | RW            ; Access
        db GRAN_4K | LONG_MODE | 0xF                ; Flags & Limit (high, bits 16-19)
        db 0                                        ; Base (high, bits 24-31)
    .Data: equ $ - GDT
        dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
        db 0                                        ; Base (mid, bits 16-23)
        db PRESENT | NOT_SYS | RW                   ; Access
        db GRAN_4K | SZ_32 | 0xF                    ; Flags & Limit (high, bits 16-19)
        db 0                                        ; Base (high, bits 24-31)
    .TSS: equ $ - GDT
        dd 0x00000068
        dd 0x00CF8900
    .Pointer:
        dw $ - GDT - 1
        dq GDT - KERNEL_START
