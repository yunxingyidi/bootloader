#ifndef PLATFORM_H
#define PLATFORM_H

#include "devices/uart.h"
#include "devices/spi.h"

//typedef unsigned int uint32_t;
//typedef unsigned long long uint64_t;


#define read_mcause(dest) \
    asm volatile ("csrr %0, mcause" : "=r" (dest))
#define write_mtvec(value) \
    asm volatile ("csrw mtvec, %0" : : "r" (value))

#define _REG32(p, i) (*(volatile uint32_t *)((p) + (i)))
#define _REG16(p, i) (*(volatile uint16_t *)((p) + (i)))

#define _AC(X,Y)        (X##Y)
//the address of Model_Select
#define MODESELECT_MEM_ADDR _AC(0x1000,UL)
#define MEMORY_MEM_ADDR _AC(0x80000000,UL)
#define SPI_CTRL_ADDR _AC(0x10040000,UL)
#define UART_CTRL_ADDR _AC(0x10010000,UL)
#define UART_REG(offset) _REG32(UART_CTRL_ADDR, offset)

#endif
