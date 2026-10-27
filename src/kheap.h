#ifndef HEAP_H
#define HEAP_H
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
void kheap_init(void);
void kfree(uintptr_t ptr);

uintptr_t kmalloc(size_t size);
#endif
