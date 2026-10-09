#include "uart.h"
#include "kprintf.h"
#include <stdint.h>
uint64_t get_current_el(void);

void exception_handler(uint64_t elr, uint64_t esr){ // elr tells us where the exception happened; esr tells us why it happened, both values are hex
                                                    
  kprintf("EXCEPTION: \nELR_E1: 0x%x\n",elr);
  kprintf("ESR_E1: 0x%x\n",esr);

  uint64_t ec = (esr >> 26) & (0x3F); //the type of exception lives in the [31:26] bits so we right shift by 26 so that the first 6 bits are these ones. Masking with 0x3F 
                                     //helps us to get to know exactly which bit of the six were 1, i.e which type of exception
  kprintf("Exception class: 0x%x\n",ec);
  
  if(ec == 0x3C){
    uart_put_string("TYPE: BRK\n");
    uint64_t comment = (esr) & (0xFFFF); //value of BRK is stored in first sixteen bits of esr
    kprintf("BRK value: %d\n",comment);
  }

}

void kernel_main(void)
{
    uart_put_string("UART console ready\n");

    uint64_t el=get_current_el(); //gets the current EL level from boot.S
    kprintf("Current EL: %d\n",el);

    uart_put_string("Before Exception\n");
    __asm__ volatile ("brk #0"); //intentionally invoking a syn spx error
    uart_put_string("After Exception\n");
    while (1) {
    }
}
