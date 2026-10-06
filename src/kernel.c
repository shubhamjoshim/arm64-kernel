#include "uart.h"

void kernel_main(void){
  uart_put_string("Booted\n");
  while(1){
  }
}
