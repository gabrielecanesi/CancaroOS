ENTRY(start)
SECTIONS
 {
   . = 1M;


   .boot ALIGN(4K) :
   {
     KEEP(*(.multiboot_header))
   }

   _kernel_start = 0xffffffff80000000;
    . += _kernel_start;

   .text ALIGN(4K) : AT (ADDR(.text) - _kernel_start)
   {
     KEEP(*(.text))
   }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - _kernel_start){
        KEEP(*(.rodata))
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - _kernel_start) {
        KEEP(*(COMMON))
        KEEP(*(.bss))
    }

    .data ALIGN(4K) : AT(ADDR(.data) - _kernel_start) {
        KEEP(*(.data))
    }



   _kernel_end = .;

 }
