#include "kprintf.h"
#include "uart.h"
#include <stdarg.h>
#include <stdint.h>

void kprintf(const char* format, ...){
  va_list args;
  va_start(args,format);
  while(*format != '\0'){
    if(*format == '%'){
      format++;
      if(*format == '%') uart_put_char('%');

      else if(*format == 's'){
        const char *string = va_arg(args,const char *);
        uart_put_string(string);
      }

      else if(*format == 'd'){
        int number=va_arg(args,int);
        uart_put_decimal(number);
      }
      
      else if(*format == 'x'){
        uint64_t hex = va_arg(args,uint64_t);
        uart_put_hex(hex);
      }

      else if(*format == 'c'){
        int c = va_arg(args,int);
        uart_put_char((char) c);
      }
      
    }
    else{
      uart_put_char(*format);
    }

    format++;
     
  }
  va_end(args);
  return;
}

