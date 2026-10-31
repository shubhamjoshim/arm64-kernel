#include "kheap.h"
#include "pmm.h"
#include <stddef.h>
#include <stdint.h>
#define PAGE_SIZE 0x1000UL

static struct heap_block *heap_head;

struct heap_block{
  size_t size;
  bool free;
  struct heap_block *next;
};

typedef struct heap_block heap;

static size_t header_size = sizeof(heap);


int canBeJoined(heap *current,size_t requested_size);
void join_blocks(heap *current){

  if(current == NULL) return;
  if(current -> free != 1) return;
  if(current -> next -> free != 1) return;

  current -> size = current -> size + current -> next -> size + header_size;
  current -> next = current -> next -> next;

  return;
}

 
void kheap_init(void){
  heap_head =(struct heap_block *) pmm_allocate_page(); 
  if(heap_head == 0) return;
  size_t usable_size =  PAGE_SIZE - sizeof(struct heap_block);
  heap_head -> size = usable_size;
  heap_head -> free = 1;
  heap_head -> next = NULL;

}

uintptr_t kmalloc(size_t requested_size){
  size_t header_size = sizeof(heap);
  heap *last = NULL;
  if(requested_size == 0) return 0;
  if(requested_size> (PAGE_SIZE - header_size)) return 0;
  if(heap_head == NULL) return 0;

  requested_size = (requested_size + 7) & ~7UL; // padding it to the nearest power of 2
  heap *current = heap_head;

  while(current != NULL){
    if((current -> size >= requested_size) && current -> free){
      if(current -> size >= (requested_size + header_size + 8)){
        size_t old_size = current -> size;
        heap *new = (heap *) ((uintptr_t)current + requested_size + header_size);

        new -> free = true;
        new -> next = current -> next;
        current -> next = new;
        current -> size  = requested_size;
        new -> size = old_size - requested_size - header_size;
      }
      current -> free = false;
      return (uintptr_t)(current) + header_size;
    }
    last = current;
    current = current -> next;
  }

  heap *new_page = (heap *) pmm_allocate_page();
  if(new_page == NULL ) return 0;
  new_page -> size = PAGE_SIZE - header_size;  
  new_page -> free = true;
  new_page ->next = NULL;
  last -> next = new_page;

  heap *new = (struct heap_block *)((uintptr_t) new_page + header_size + requested_size);

  if(new_page -> size >= requested_size + header_size + 8){
    size_t old_size = new_page -> size;
    new -> size = old_size - requested_size - header_size;
   
    new -> free = true;
    new -> next = NULL;

    new_page -> size = requested_size;
    new_page -> next = new;
  }
  new_page -> free = false;
  return (uintptr_t) new_page + header_size;
}

void kfree(uintptr_t ptr){
  if(ptr == 0) return;

  heap *header = (heap *) (ptr - header_size);
  header -> free = true;
  heap *temp = heap_head;
  heap *prev = NULL;

  while(temp != NULL && temp != header){
    prev = temp;
    temp = temp -> next;
  }

  while (header -> next != NULL){
    uintptr_t expected_next = (uintptr_t) header + header_size + header -> size;

    if(!header -> next -> free) break;
    if(expected_next != (uintptr_t)header -> next) break;
    if(expected_next == (uintptr_t) header -> next){
        header -> size = (header -> size + header -> next -> size + header_size);
        header -> next = header -> next -> next;
    }
  }

  if(prev != NULL && prev -> free){
    uintptr_t expected_prev = (uintptr_t) header - prev -> size - header_size;

    if((uintptr_t)prev == expected_prev){
      prev -> size = prev -> size + header_size + header -> size;
      prev -> next = header -> next;
    }
  }

}



int canBeJoined(heap *current, size_t requested_size){
  if(current == NULL) return -1;
  if(current -> free != 1 || current -> next -> free != 1) return -1;
  
  if(current -> size + current -> next -> size >= requested_size) return 1;

  else {
    return 0;
  }
}
