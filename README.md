# ARM64 Kernel
An experimental ARM64 kernel written in C and AArch64 assembly.

## Features (current)
 - UART driver
 - Basic Exception handling
 - Physical page allocator
 - A working Memory Management Unit (MMU)
 - Kernel heap (malloc)

## Roadmap
 - [x] Boot and enter C at EL1
 - [x] PL011 UART and kernel printing
 - [x] Exception Vectors and exception handling
 - [x] ARM generic timer and GIC interrupts
 - [x] Physical page allocator
 - [x] Page tables
 - [x] Virtual memory and mapping
 - [x] MMU
 - [x] Kernel heap (`kmalloc`,`kfree`,splitting,coalescing)
 - [ ] Tasks and Kernel stacks
 - [ ] Context Switching
 - [ ] Preemptive Scheduling
 - [ ] Userspace / EL0
 - [ ] System Calls
 - [ ] ELF Loader
 - [ ] Simple filesystem
 - [ ] Userspace shell

 **Goal:** boot from the CPU's first instruction to a userspace program running through the kernel and making system calls.

## Installation and Running

### Prerequisites
The kernel is developed on macOS using:
 - GNU make
 - AArch64 bare-metal gcc toolchain
 - QEMU with AArch64 system emulation

1. Install apple command line tools first: `xcode-select --install`
2. Install AArch64 Cross-compiler : `brew install aarch64-elf-gcc`
3. Install QEMU: `brew install qemu`
4. Make sure QEMU has support for AArch64 emulation: `qemu-system-aarch64 --version`

### Running

1. Clone the repository in your desired directory.
    `git clone https://github.com/shubhamjoshim/arm64-kernel`
2. Compile all the files using make to make sure there are no errors first.
    `make`
3. Run `make run` to run the kernel.
4. For quitting, press Ctrl+A, then X.

