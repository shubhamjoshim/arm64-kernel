.PHONY: all run
all: build/kernel.elf
build/boot.o: src/boot.S 
	aarch64-elf-gcc -c src/boot.S -o build/boot.o
build/kernel.o: src/kernel.c src/uart.h
	aarch64-elf-gcc -ffreestanding -Wall -Wextra -Wpedantic -c src/kernel.c -o build/kernel.o
build/uart.o: src/uart.c src/uart.h
	aarch64-elf-gcc -ffreestanding -Wall -Wextra -Wpedantic -c src/uart.c -o build/uart.o
build/vector.o: src/vector.S 
	aarch64-elf-gcc -ffreestanding -Wall -Wextra -Wpedantic -c src/vector.S -o build/vector.o
build/kernel.elf: build/boot.o build/vector.o build/kernel.o build/uart.o linker.ld
	aarch64-elf-ld -T linker.ld build/vector.o build/boot.o build/kernel.o build/uart.o -o build/kernel.elf
run: build/kernel.elf
	qemu-system-aarch64 -machine virt -cpu cortex-a53 -nographic -kernel build/kernel.elf

