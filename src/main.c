#include "uart.h"
#include "spi.h"
#include "ux00boot.h"

#define CORE_CLK_KHZ 33000
#define UART_CTRL UART_CTRL_ADDR
//TODO the size of the payload
#define UART_READ_SIZE 0
#define SPI_READ_ADDR 0
#define SPI_READ_SIZE 0

void handle_trap(void)
{
  uint32_t value;
  read_mcause(value);
  ux00boot_fail((long) value, 1);
}

void jumpToSRAM()
{
    void* address = (void*)MEMORY_MEM_ADDR;
    asm volatile (
        "jalr %[address]"  // 使用jalr指令进行跳转
        :
        : [address] "r" (address)
    );
}

void init_uart(unsigned int peripheral_input_khz)
{
  unsigned long long uart_target_hz = 115200ULL;
  UART_REG(UART_REG_DIV) = uart_min_clk_divisor(peripheral_input_khz * 1000ULL, uart_target_hz);
}

void useUart()
{
    unsigned int peripheral_input_khz;
    peripheral_input_khz = CORE_CLK_KHZ; 
    init_uart(peripheral_input_khz);
    uart_read(UART_CTRL);
}

void useSPI()
{
    spi_ctrl* spictrl = (void*) 0;
    spictrl = (spi_ctrl*) SPI_CTRL_ADDR;
    spi_copy(spictrl, (void*)MEMORY_MEM_ADDR, (void*)SPI_READ_ADDR, SPI_READ_SIZE);
}

int main()
{
    uint32_t mode_select = *((volatile uint32_t*) MODESELECT_MEM_ADDR);

    switch (mode_select)
    {
    case 0:
        jumpToSRAM();
        break;
    case 1:
        useUart();
    case 2:
        useSPI();
    default:
        ux00boot_fail(0x1, 0);
        break;
    }
    write_mtvec(MEMORY_MEM_ADDR);
    asm volatile (
        "unimp"
    );
    return 0;
}