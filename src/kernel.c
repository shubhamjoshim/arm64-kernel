#include "uart.h"
#include <stdint.h>
uint64_t get_current_el(void);

void exception_handler(uint64_t elr, uint64_t esr){
  uart_put_string("Exception: \n");
  uart_put_string("ELR_E1: 0x");
  uart_put_hex(elr);
  uart_put_char('\n');

  uart_put_string("ESR_E1: 0x");
  uart_put_hex(esr);
  uart_put_char('\n');

  uint64_t ec = (esr >> 26) & (0x3F); //the type of exception lives in the [31:26] bits so we right shift by 26 so that the first 6 bits are these ones. Masking with 0x3F 
                                     //helps us to get to know exactly which bit of the six were 1, i.e which type of exception
  uart_put_string("Exception class: 0x");
  uart_put_hex(ec);
  uart_put_char('\n');
  
  if(ec == 0x3C){
    uart_put_string("TYPE: BRK\n");
    uint64_t comment = (esr) & (0xFFFF); //value of BRK is stored in first sixteen bits of esr
    uart_put_string("BRK value: ");
    uart_put_decimal(comment);
    uart_put_char('\n');
  }

}

void kernel_main(void)
{
    uart_put_string("UART console ready\n");

    uint64_t el=get_current_el();
    uart_put_string("Current EL: ");
    uart_put_decimal(el);
    uart_put_char('\n');

    uart_put_string("Before Exception\n");
    __asm__ volatile ("brk #0");
    uart_put_string("After Exception\n");
    while (1) {
    }
}
