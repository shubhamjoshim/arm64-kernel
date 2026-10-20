#ifndef VMM_H
#define VMM_H
#include "pmm.h"
void set_zero(uintptr_t table);
void vmm_init();
void vmm_test_mapping(void);
uint64_t extract_l0(uintptr_t va);
#endif
