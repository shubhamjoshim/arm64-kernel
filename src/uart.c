#include <stdint.h>
#include "uart.h"
#define UART0_BASE 0x09000000UL
#define UART_DATA_OFFSET 0x00UL
#define UART_FLAG_OFFSET 0x18UL

#define UART_FR_TXFF (1U << 5)


volatile uint32_t *uart_fr = (volatile uint32_t *)(UART0_BASE + UART_FLAG_OFFSET);
volatile uint32_t *uart_dr = (volatile uint32_t *)(UART0_BASE + UART_DATA_OFFSET);

void uart_put_char(char c){
  while(*uart_fr & UART_FR_TXFF){

  }
  *uart_dr = (uint32_t) c ;
}

void uart_put_string(const char *s){
  while(*uart_fr & UART_FR_TXFF){

  }
  while(*s != '\0'){
    uart_put_char(*s);
    s++;
  }
}
