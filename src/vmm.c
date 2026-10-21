
#include "pmm.h"
#include <stddef.h>
#include <stdint.h>
#include "vmm.h"
#define NORMAL_FLAGS  0x703ULL
#define DEVICE_FLAGS 0x407ULL
static uintptr_t page_table_l0;
static uint64_t *l0;

extern void setup_mair(void);
extern void prepare_mmu(void);
extern void setup_tcr(void);
extern uint64_t read_mair(void);
extern void setup_ttbr0(uintptr_t address);
extern uint64_t read_ttbr0(void);
extern void enable_mmu(void);

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


  map_region(0x40000000ULL,0x40000000ULL,0x08000000ULL,NORMAL_FLAGS); //map the entire RAM
  map_page(0x09000000ULL, 0x09000000ULL, DEVICE_FLAGS); // UART
  map_page(0x08000000ULL, 0x08000000ULL, DEVICE_FLAGS); // GIC distributor
  map_page(0x08010000ULL, 0x08010000ULL, DEVICE_FLAGS); // GIC CPU interface
                                                        //
  setup_tcr();
  setup_ttbr0(page_table_l0);                                                       //
  setup_mair();
  prepare_mmu();
  enable_mmu();
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

void map_page(uintptr_t va, uintptr_t pa, uint64_t flags){

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


  current_l3[l3_index] = (uint64_t)pa | flags;
}

void map_region(uintptr_t va, uintptr_t pa, size_t size, uint64_t flags){
  if((pa & 0xFFFULL) != 0) return;
  if((va & 0xFFFULL) != 0) return;
  if(size == 0) return;
  size = (size_t) align_up((uintptr_t)size);

  while(size > 0){

    map_page(va, pa,flags);

    va += 0x1000;
    pa += 0x1000;
    size -= 0x1000;
  }

}

