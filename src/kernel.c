#include "uart.h"

void kernel_main(void)
{
    uart_put_string("UART console ready\n");

    uart_put_string("Character: ");
    uart_put_char('A');
    uart_put_char('\n');

    uart_put_string("Hex: 0x");
    uart_put_hex(0x40080000);
    uart_put_char('\n');

    uart_put_string("Decimal: ");
    uart_put_decimal(12345);
    uart_put_char('\n');

    while (1) {
    }
}
