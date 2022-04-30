global header_start
global framebuffer_tag_start
section .multiboot_header
header_start:
align 8
	; magic number
	dd 0xe85250d6 ; multiboot2
	; architecture
	dd 0 ; protected mode i386
	; header length
	dd header_end - header_start
	; checksum
	dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))


	; graphics mode
framebuffer_tag_start:
align 8
    dw  0x05    ;Type: framebuffer
    dw  0x01
    dd  framebuffer_tag_end - framebuffer_tag_start ;size
    dd  1024   ;Width
    dd  768   ;Height
    dd  32   ;Depth
framebuffer_tag_end:

end_tag_start:
align 8
	; end tag
	dw 0
	dw 0
	dd 8
end_tag_end:

header_end:
