#include "uart.h"
#include <stdint.h>
uint64_t get_current_el(void);

void kernel_main(void)
{
    uart_put_string("UART console ready\n");

    uint64_t el=get_current_el();
    uart_put_string("Current EL: ");
    uart_put_decimal(el);
    uart_put_char('\n');



    while (1) {
    }
}
