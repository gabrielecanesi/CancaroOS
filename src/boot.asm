extern idtable
global refresh_tlb
global h_pages
global page_l4
global page_fb
%define KERNEL_HIGH_VMA 0xffffffff80000000

section .bss
align 16
stack:
	; allocate 16384 bytes, the stack grows downwards on x86
	resb 8192
	.top:

; macro to setup pagetables later
%macro gen_pd_2mb 3
	%assign i %1
	%rep %2
		dq (i | 0x83)
		%assign i i+0x200000
	%endrep
	%rep %3
		dq 0
	%endrep
%endmacro


section .data

section .text
global start
start:
	; The bootloader has loaded us into 32-bit protected mode on an x86 machine.
	; Interrupts are disabled. Paging is disabled. The processor state is as
	; defined in the multiboot standard. The kernel has full control of the CPU.

	; move the stack pointer to the top of our newly created stack
	mov edi, ebx
	mov esp, stack.top - KERNEL_HIGH_VMA

	bits 32

    push eax
    push ebx

    mov ecx, 0

	; load page tables
	mov eax, page_l4 - KERNEL_HIGH_VMA
	mov cr3, eax	; set control register 3 to the level 4 page table

	; enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; long mode bit
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; enable paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	; load the new GDT
	lgdt [gdt.ptr_low - KERNEL_HIGH_VMA]


    pop ebx
    pop eax

	; load new code selector (long jump)
	jmp (0x8):(start64 - KERNEL_HIGH_VMA)
	bits 64

start64:
	bits 64
	mov cx, 0x0
	mov ss, cx
	mov ds, cx
	mov es, cx
	mov fs, cx
	mov gs, cx

	; jump to the higher half kernel
	mov rcx, higher_half
	jmp rcx

higher_half:
	mov rsp, stack.top
	mov rcx, 0
    mov [page_l4], rcx

    mov rcx, cr3
    mov cr3, rcx

	lgdt [gdt.ptr]


    mov rdi, rax
    mov rsi, rbx

	; call out to kmain
	extern kernel_main
	extern initMultiboot
	call initMultiboot
	call kernel_main
	cli
.hang: hlt
	jmp .hang
;.end

load_idt:
    lidt [idtable - KERNEL_HIGH_VMA]
    ret


refresh_tlb:
    mov rax, cr3
    mov cr3, rax
    ret

h_pages:
    mov cr3, rdi
section .data
;
; Define a static page table in Assembly just for now
; This should map 1 GB total
;
align 4096
page_l4:
	; map lower half of memory
	dq page_l3 + 0x3 - KERNEL_HIGH_VMA
	times 509 dq 0 ; padding

	dq page_l3_fb + 0x3 - KERNEL_HIGH_VMA
	; map higher half of memory
	dq page_l3_h + 0x3 - KERNEL_HIGH_VMA
page_l3:
	; map lower half of memory
	dq page_l2 + 0x3 - KERNEL_HIGH_VMA
	times 510 dq 0 ; padding
	dq page_l2 + 0x3 - KERNEL_HIGH_VMA
page_l3_h:
	times 510 dq 0 ; padding
	;dq page_fb + 0x3 - KERNEL_HIGH_VMA

	; map higher half of memory
	dq page_l2 + 0x3 - KERNEL_HIGH_VMA
	dq 0

page_l2:
	; identity map 1 GB
	;gen_pd_2mb 0, 10, 502
	gen_pd_2mb 0, 512, 0
page_fb:
    times 512 dq 0

page_l3_fb:
    dq page_fb + 0x3 - KERNEL_HIGH_VMA
    times 511 dq 0
;
; Define a static GDT in Assembly
; We need this (i think) so we can get into higher half mode, this will be replaced in C.
;
gdt:
	.null: equ $ - gdt
		dw 0xFFFF				; limit (low)
		dw 0					; base (low)
		db 0					; base (mid)
		db 0					; access
		db 0					; granularity
		db 0					; base (high)

	.kernel_code: equ $ - gdt
		dw 0
		dw 0
		db 0
		db 10011010b			; access (exec/read)
		db 10101111b			; granularity
		db 0

	.kernel_data: equ $ - gdt
		dw 0
		dw 0
		db 0
		db 10010010b			; access (read/write)
		db 0
		db 0

	.user_code: equ $ - gdt
		dw 0
		dw 0
		db 0
		db 11111010b			; access (exec/read)
		db 10101111b			; granularity
		db 0

	.user_data: equ $ - gdt
		dw 0
		dw 0
		db 0
		db 10010010b			; access (read/write)
		db 0
		db 0

	.ptr:
		dw $ - gdt - 1			; limit
		dq gdt					; base

	.ptr_low:					; 32 bit GDT pointer
		dw $ - gdt - 1			; limit
		dq gdt - KERNEL_HIGH_VMA; base