#ifndef HEAP_H
#define HEAP_H
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
void kheap_init(void);
uintptr_t kmalloc(size_t size);
#endif
