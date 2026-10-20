#include "pmm.h"
#include "kprintf.h"
#include <stdint.h>
static uintptr_t page_table_l0;
static uint64_t *l0;
static uintptr_t page_table_l1;
static uint64_t *l1;
static uintptr_t page_table_l2;
static uint64_t *l2;
static uintptr_t page_table_l3;
static uint64_t *l3;

extern void setup_mair(void);
extern uint64_t read_mair(void);

void set_zero(uintptr_t table){
  uint64_t *entires = (uint64_t *) table;
  for(int i=0;i<512;i++){
    (*entires) = 0;
    entires++;
  }

}

uintptr_t give_page(void){
  uintptr_t new = pmm_allocate_page();
  if(new == 0) return 0;
  set_zero(new);
  new = new | 0x3ULL;
  return new;
}
void vmm_init(){
   page_table_l0 = pmm_allocate_page();
   if(page_table_l0 == 0) return;
   l0 = (uint64_t *) page_table_l0;
   set_zero(page_table_l0);

   page_table_l1 = pmm_allocate_page();
   if(page_table_l1 == 0) return;
   set_zero(page_table_l1);
   l1 = (uint64_t *) page_table_l1;

   l0[0] = page_table_l1 | 0x3ULL;

   page_table_l2 = pmm_allocate_page();
   if(page_table_l2 == 0) return;
   set_zero(page_table_l2);
   l2 = (uint64_t *) page_table_l2;

   l1[0] = page_table_l2 | 0x3ULL;


   page_table_l3 = pmm_allocate_page();
   if(page_table_l3 == 0) return;
   set_zero(page_table_l3);
   l3 = (uint64_t *) page_table_l3;

   l2[0] = page_table_l3 | 0x3ULL;

   setup_mair();
}

uint64_t extract_l0(uintptr_t va){
  va = va >> 39;
  va = va & (0x1FF);
  return va;
}

uint64_t extract_l1(uintptr_t va){
  va = va >> 30;
  va = va & (0x1FF);
  return va;
}

uint64_t extract_l2(uintptr_t va){
  va = va >> 21;
  va = va & (0x1FF);
  return va;
}

uint64_t extract_l3(uintptr_t va){
  va = va >> 12;
  va = va & (0x1FF);
  return va;
}

uint64_t extract_offset(uintptr_t va){
  va = va & (0xFFF);
  return va;
}

void remove_descriptor_bits(uintptr_t *entry){
  *entry = *entry & (~0xFFFULL);
  return;
}
void map_page(uintptr_t va, uintptr_t pa){

  if((pa & 0xFFFULL) != 0) return;
  if((va & 0xFFFULL) != 0) return;

  uint64_t l0_index = extract_l0(va);
  uint64_t entry = l0[l0_index];
  if((entry & 0x1ULL) == 0){
    uintptr_t new = give_page();
    if(new == 0) return;
    entry = new;
    l0[l0_index] = entry;
  }
  uintptr_t temp = (entry & ~0xFFFULL);
  uint64_t *current_l1 = (uint64_t *)temp;

  uint64_t l1_index = extract_l1(va);
  entry = current_l1[l1_index];
  if((entry & 1ULL) == 0){
    uintptr_t new = give_page();
    if(new == 0) return;
    entry = new;
    current_l1[l1_index] = entry;
  }
  temp = entry & ~0xFFFULL;
  uint64_t *current_l2 = (uint64_t *) temp;

  uint64_t l2_index = extract_l2(va);
  entry = current_l2[l2_index];
  if((entry & 1ULL) == 0){
    uintptr_t new = give_page();
    if(new == 0) return;
    entry = new;
    current_l2[l2_index] = entry;
  }
  temp = entry & ~0xFFFULL;
  uint64_t *current_l3 = (uint64_t *) temp;

  uint64_t l3_index = extract_l3(va);

  uint64_t flags = (1ULL << 0) | (1ULL << 1) | (3ULL << 8) | (1ULL << 10);

  current_l3[l3_index] = (uint64_t)pa | flags;
}

