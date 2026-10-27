.PHONY: all run

all: build/kernel.elf

build/boot.o: src/boot.S
	aarch64-elf-gcc -c src/boot.S -o build/boot.o

build/vector.o: src/vector.S
	aarch64-elf-gcc -c src/vector.S -o build/vector.o
build/timer.o: src/timer.S
	aarch64-elf-gcc -c src/timer.S -o build/timer.o
build/kernel.o: src/kernel.c src/uart.h src/kprintf.h src/pmm.h src/timer.h src/gic.h
	aarch64-elf-gcc -ffreestanding -mgeneral-regs-only -Wall -Wextra -Wpedantic -c src/kernel.c -o build/kernel.o

build/gic.o: src/gic.c src/gic.h
	aarch64-elf-gcc -ffreestanding -mgeneral-regs-only -Wall -Wextra -Wpedantic -c src/gic.c -o build/gic.o
build/uart.o: src/uart.c src/uart.h
	aarch64-elf-gcc -ffreestanding -mgeneral-regs-only -Wall -Wextra -Wpedantic -c src/uart.c -o build/uart.o
build/pmm.o: src/pmm.c src/pmm.h
	aarch64-elf-gcc -ffreestanding -mgeneral-regs-only -Wall -Wextra -Wpedantic -c src/pmm.c -o build/pmm.o
build/kprintf.o: src/kprintf.c src/kprintf.h src/uart.h
	aarch64-elf-gcc -ffreestanding -mgeneral-regs-only -Wall -Wextra -Wpedantic -c src/kprintf.c -o build/kprintf.o

build/vmm.o: src/vmm.c src/vmm.h src/pmm.h src/kprintf.h
	aarch64-elf-gcc -ffreestanding -mgeneral-regs-only -Wall -Wextra -Wpedantic -c src/vmm.c -o build/vmm.o
build/kheap.o: src/kheap.c src/kheap.h src/pmm.h
	aarch64-elf-gcc -ffreestanding -mgeneral-regs-only -Wall -Wextra -Wpedantic -c src/kheap.c -o build/kheap.o

build/mmu.o: src/mmu.S
	aarch64-elf-gcc -c src/mmu.S -o build/mmu.o

build/kernel.elf: build/boot.o build/vector.o build/pmm.o build/timer.o build/kernel.o  build/uart.o build/kprintf.o build/gic.o build/vmm.o build/mmu.o build/kheap.o linker.ld
	aarch64-elf-ld -T linker.ld build/boot.o build/vector.o build/pmm.o build/timer.o build/kernel.o build/gic.o build/uart.o build/kprintf.o build/vmm.o build/mmu.o build/kheap.o -o build/kernel.elf

run: build/kernel.elf
	qemu-system-aarch64 -machine virt -cpu cortex-a53 -nographic -kernel build/kernel.elf
