# ARM64 Kernel
An experimental ARM64 kernel written in C and AArch64 assembly.

## Features (current)
 - UART driver
 - Basic Exception handling
 - Physical page allocator
 - A working Memory Management Unit (MMU)
 - Kernel heap (malloc)

## Installation and Running

### Prerequisites
The kernel is developed on macOS using:
 - GNU make
 - AArch64 bare-metal gcc toolchain
 - QEMU with AArch64 system emulation

1. Install apple command line tools first: `xcode-select --install`
2. Install AArch64 Cross-compiler : `brew install aarch64-elf-gcc`
3. Install QEMU: `brew install qemu`
4 Make sure QEMU has support for AArch64 emulation: `qemu-system-aarch64 --version`

### Running

1. Clone the repository in your desired directory.
    `git clone https://github.com/shubhamjoshim/arm64-kernel`
2. Compile all the files using make to make sure there are no errors first.
    `make`
3. Run `make run` to run the kernel.
4. For quitting, press Ctrl+A, then X.

