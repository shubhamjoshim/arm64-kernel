#ifndef PMM_H
#define PMM_H
#include <stdint.h>
uintptr_t pmm_allocate_page(void);
uintptr_t align_up();
void pmm_init(void);
#endif
