#include <stdint.h>
#define PAGE_SIZE 0x1000UL
#define RAM_END 0x48000000UL

extern char kernel_end[];
static uintptr_t next_free_page;

uintptr_t align_up(uintptr_t address){
  uintptr_t next;
  next = ( address + PAGE_SIZE -1) & ~(PAGE_SIZE - 1);
  return next;
}

void pmm_init(void){
  next_free_page = align_up((uintptr_t)kernel_end);
}

uintptr_t pmm_allocate_page(void){
  if(next_free_page >= RAM_END){
    return 0;
  }
  uintptr_t page = next_free_page;
  next_free_page += PAGE_SIZE;
  return page;
}
