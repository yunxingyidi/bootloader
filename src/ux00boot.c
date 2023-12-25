#include "ux00boot.h"
#include "uart.h"

// Bit fields of error codes
#define ERROR_CODE_BOOTSTAGE (0xfUL << 60)
#define ERROR_CODE_TRAP (0xfUL << 56)
#define ERROR_CODE_ERRORCODE ((0x1UL << 56) - 1)
// Bit fields of mcause fields when compressed to fit into the errorcode field
#define ERROR_CODE_ERRORCODE_MCAUSE_INT (0x1UL << 55)
#define ERROR_CODE_ERRORCODE_MCAUSE_CAUSE ((0x1UL << 55) - 1)

#define ERROR_CODE_UNHANDLED_SPI_DEVICE 0x1
#define ERROR_CODE_UNHANDLED_BOOT_ROUTINE 0x2
#define ERROR_CODE_GPT_PARTITION_NOT_FOUND 0x3
#define ERROR_CODE_SPI_COPY_FAILED 0x4
#define ERROR_CODE_SD_CARD_CMD0 0x5
#define ERROR_CODE_SD_CARD_CMD8 0x6
#define ERROR_CODE_SD_CARD_ACMD41 0x7
#define ERROR_CODE_SD_CARD_CMD58 0x8
#define ERROR_CODE_SD_CARD_CMD16 0x9
#define ERROR_CODE_SD_CARD_CMD18 0xa
#define ERROR_CODE_SD_CARD_CMD18_CRC 0xb
#define ERROR_CODE_SD_CARD_UNEXPECTED_ERROR 0xc
#define INSERT_FIELD(val, which, fieldval) (((val) & ~(which)) | ((fieldval) * ((which) & ~((which)-1))))


void ux00boot_fail(long code, int trap)
{
    // Print error code to UART
    UART_REG(UART_REG_TXCTRL) = UART_TXEN;

    // Error codes are formatted as follows:
    // [63:60]    [59:56]  [55:0]
    // bootstage  trap     errorcode
    // If trap == 1, then errorcode is actually the mcause register with the
    // interrupt bit shifted to bit 55.
    uint64_t error_code = 0;
    if (trap) {
    error_code = INSERT_FIELD(error_code, ERROR_CODE_ERRORCODE_MCAUSE_CAUSE, code);
    if (code < 0) {
        error_code = INSERT_FIELD(error_code, ERROR_CODE_ERRORCODE_MCAUSE_INT, 0x1UL);
    }
    } else {
    error_code = code;
    }
    uint64_t formatted_code = 0;
    formatted_code = INSERT_FIELD(formatted_code, ERROR_CODE_TRAP, trap);
    formatted_code = INSERT_FIELD(formatted_code, ERROR_CODE_ERRORCODE, error_code);

    uart_puts((void*) UART_CTRL_ADDR, "Error 0x");
    uart_put_hex((void*) UART_CTRL_ADDR, formatted_code >> 32);
    uart_put_hex((void*) UART_CTRL_ADDR, formatted_code);

    while (1);
}