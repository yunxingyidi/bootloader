#ifndef _DRIVERS_UART_H
#define _DRIVERS_UART_H
#include <platform.h>

#ifndef __ASSEMBLER__

void uart_putc(void* uartctrl, char c);
char uart_getc(void* uartctrl);
void uart_puts(void* uartctrl, const char * s);
void uart_put_hex(void* uartctrl, uint32_t hex);
void uart_put_hex64(void* ua64ctrl, uint64_t hex);
void uart_read(void* uartctrl);

#endif /* !__ASSEMBLER__ */

#endif /* _DRIVERS_UART_H */
