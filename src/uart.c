#include <stdint.h>
#include "uart.h"
#define UART0_BASE 0x09000000UL
#define UART_DATA_OFFSET 0x00UL
#define UART_FLAG_OFFSET 0x18UL


#define UART_FR_TXFF (1U << 5) //fifo full bit


static volatile uint32_t *const uart_fr = (volatile uint32_t *)(UART0_BASE + UART_FLAG_OFFSET); //setting the address of the flag byte
static volatile uint32_t * const uart_dr = (volatile uint32_t *)(UART0_BASE + UART_DATA_OFFSET); //setting the address of the data byte

void uart_put_char(char c){
  while(*uart_fr & UART_FR_TXFF){
    //loop runs till the fifo is not full, checks continuously
  }
  if(c == '\n'){
    *uart_dr = (uint32_t) '\r';
    while(*uart_fr & UART_FR_TXFF){
        //loop runs till the fifo is not full, checks continuously
      }
  }
  *uart_dr = (uint32_t) c ; //changing the value of uart_dr (Data register)
}

void uart_put_string(const char *s){
  //not using UART_FR_TXFF and uart_fr check as it is done in uart_put_char
  while(*s != '\0'){
    uart_put_char(*s);
    s++;
  }
}

void uart_put_hex(uint64_t number){
  static const char digits[]= "0123456789ABCDEF";

  for(int i = 60; i>=0 ; i-=4){
    uint64_t index = (number >> i) & (0xF);
    uart_put_char(digits[index]);
  }
}

void uart_put_decimal(uint64_t value){
  char number[20];
  int count=0;
  if(value == 0){
    uart_put_char('0');
    return;
  }
  while(value>0){
    number[count] = '0' + value % 10;
    value=value/10;
    count++;
  }
  while(count >0){
    count--;
    uart_put_char(number[count]);
  }
}
