#ifndef UART_H
#define UART_H
#include <stdint.h>
  void uart_put_char(char c);
  void uart_put_string(const char *s);
  void uart_put_hex(uint64_t number);
  void uart_put_decimal(uint64_t value);
#endif
