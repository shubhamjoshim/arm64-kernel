#ifndef VMM_H
#define VMM_H
#include "pmm.h"
#include <stddef.h>
void set_zero(uintptr_t table);
void vmm_init();
uint64_t extract_l0(uintptr_t va);
void map_page(uintptr_t va, uintptr_t pa, uint64_t flags);
void vmm_test_device_mapping(void);
void map_region(uintptr_t va, uintptr_t pa, size_t size, uint64_t flags);
void vmm_test_region(void);
#endif
