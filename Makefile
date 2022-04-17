SRC_DIR := ./src
OBJ_DIR := ./build

CPP_SRC := $(wildcard $(SRC_DIR)/*.cpp)
ASM_SRC := $(wildcard $(SRC_DIR)/*.asm)

CPP_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SRC))
ASM_OBJ := $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.obj,$(ASM_SRC))


kernel.iso: build/kernel.bin
	cp build/kernel.bin iso/boot
	grub-mkrescue /usr/lib/grub/i386-pc iso -o kernel.iso

build/kernel.bin: $(ASM_OBJ) $(CPP_OBJ)
	x86_64-elf-ld -n -T linker.ld $(CPP_OBJ) $(ASM_OBJ) -o build/kernel.bin


$(OBJ_DIR)/interrupts.o: $(SRC_DIR)/interrupts.cpp
	x86_64-elf-g++ -c $(SRC_DIR)/interrupts.cpp -o $(OBJ_DIR)/interrupts.o -ffreestanding -mgeneral-regs-only


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	x86_64-elf-g++ -c -o $@ $<



$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.asm
	nasm -f elf64 -o $@ $<



.PHONY: clean
clean:
	rm build/*.* iso/boot/kernel.bin