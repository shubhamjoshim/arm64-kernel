#include <stdint.h>
#include <stddef.h>
#define PAGE_SIZE 0x1000UL
#define RAM_END 0x48000000UL

struct free_page{
  struct free_page *next;
};
extern char kernel_end[];
static uintptr_t next_free_page;
static struct free_page *free_list;

void pmm_free_page(uintptr_t page){
  if(page<=(uintptr_t) kernel_end) return;
  if(page >= RAM_END) return;
  if(page % PAGE_SIZE != 0) return;
  if(page >= next_free_page) return;
  struct free_page* temp = free_list;
  int exists = 0;
  while(temp != NULL){
    if((uintptr_t) temp == page){
      exists = 1;
      break;
    }
    temp = temp-> next; 
  }
  if(exists != 0) return;
  struct free_page *node = (struct free_page *) page;
  node->next = free_list;
  free_list = node;
}


uintptr_t align_up(uintptr_t address){
  uintptr_t next;
  next = (address + PAGE_SIZE -1) & ~(PAGE_SIZE - 1);
  return next;
}

void pmm_init(void){
  next_free_page = align_up((uintptr_t) kernel_end);
  free_list = NULL;
}

uintptr_t pmm_allocate_page(void){
  if(free_list != NULL){
    struct free_page *node = free_list;
    free_list =  free_list->next;
    return (uintptr_t)node;
  }
  if(next_free_page >= RAM_END){
    return 0;
  }
  uintptr_t page = next_free_page;
  next_free_page +=PAGE_SIZE;
  return page;

}
