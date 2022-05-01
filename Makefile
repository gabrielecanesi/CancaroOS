SRC_DIR := ./src
OBJ_DIR := ./build

CPP_SRC := $(shell find $(SRC_DIR) -name '*.cpp' -print)
ASM_SRC := $(shell find $(SRC_DIR) -name '*.asm' -print)

CPP_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SRC))
ASM_OBJ := $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.obj,$(ASM_SRC))


all: pc

pc: build/kernel.bin
	cp build/kernel.bin iso-i386-pc/boot
	grub-mkrescue /usr/lib/grub/i386-pc iso-i386-pc -o kernel.iso

efi: build/kernel.bin
	mkdir -p iso-x86_64-efi/EFI/BOOT
	cp build/kernel.bin iso-x86_64-efi/boot
	grub-mkstandalone -O x86_64-efi -o iso-x86_64-efi/EFI/BOOT/BOOTX64.EFI "boot/grub/grub.cfg=grub.cfg"
	grub-mkrescue /usr/lib/grub/x86_64-efi iso-x86_64-efi -o kernel.iso

build/kernel.bin: $(ASM_OBJ) $(CPP_OBJ)
	x86_64-elf-ld -n -T linker.ld $(CPP_OBJ) $(ASM_OBJ) -o build/kernel.bin


$(OBJ_DIR)/interrupt/interrupts.o: $(SRC_DIR)/interrupt/interrupts.cpp
	mkdir -p build/interrupt
	x86_64-elf-g++ -c $(SRC_DIR)/interrupt/interrupts.cpp -o $(OBJ_DIR)/interrupt/interrupts.o -mcmodel=large -ffreestanding -mgeneral-regs-only


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	x86_64-elf-g++ -c -ggdb -mcmodel=large -o $@ $<



$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 -o $@ $<

.PHONY: clean
clean:
	rm -rf build iso-x86_64-efi/boot/kernel.bin iso-i386-pc/boot/kernel.bin kernel.iso iso-x86_64-efi/EFI/BOOT/BOOTX64.EFI