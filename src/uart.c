#include <stdatomic.h>
#include "uart.h"


void uart_putc(void* uartctrl, char c) {
#if __riscv_atomic
  int32_t r;
  do {
    asm volatile (
      "amoor.w %0, %2, %1\n"
      : "=r" (r), "+A" (_REG32(uartctrl, UART_REG_TXFIFO))
      : "r" (c)
    );
  } while (r < 0);
#else
  while ((int) _REG32(uartctrl, UART_REG_TXFIFO) << 15 < 0);
  _REG32(uartctrl, UART_REG_TXFIFO) = c;
#endif
}


char uart_getc(void* uartctrl){
  int32_t val = -1;
  while ((val << 15) < 0){
    val = (int32_t) _REG32(uartctrl, UART_REG_RXFIFO);
  }
  return val & 0xFF;
}

void uart_puts(void* uartctrl, const char * s) {
  while (*s != '\0'){
    uart_putc(uartctrl, *s++);
  }
}


void uart_put_hex(void* uartctrl, uint32_t hex) {
  int num_nibbles = sizeof(hex) * 2;
  for (int nibble_idx = num_nibbles - 1; nibble_idx >= 0; nibble_idx--) {
    char nibble = (hex >> (nibble_idx * 4)) & 0xf;
    uart_putc(uartctrl, (nibble < 0xa) ? ('0' + nibble) : ('a' + nibble - 0xa));
  }
}

void uart_put_hex64(void *uartctrl, uint64_t hex){
  uart_put_hex(uartctrl,hex>>32);
  uart_put_hex(uartctrl,hex&0xFFFFFFFF);
}

// void uart_read(uint32_t size, void* dst, void* uartctrl) {
//   uint8_t* dst_bytes = (uint8_t*) dst;
//   for(unsigned int i = 0; i < size; i++){
//       *dst_bytes = uart_getc(uartctrl);
//       dst_bytes++;
//   }
// }
/**
 * @Author: zhang
 * @Description: Read and create a list from the UART information. Get the number of 
 * the segment of the executable file after the first frame;
 */
void createUartPage(void* uartctrl, uartPage* p)
{
  char num;
  num = uart_getc(uartctrl);
  p->segnum = (int)num;
}
/**
 * @Author: zhang
 * @Description: Read and store the different segment 
 * which has different size of the executable file into different address
 */
void uartRead(void* uartctrl)
{
  char val;
  val = uart_getc(uartctrl);
  while(val != UART_START_FRAME) {
    val = uart_getc(uartctrl);
  }
  uartPage page;
  uartPage *p = &page;
  createUartPage(uartctrl, p);
  for(int i = 0; i < page.segnum; i++) {
    while(val != UART_START_FRAME) {
      val = uart_getc(uartctrl);
    }
    uartSeg *s = getinformation(uartctrl);
    getdata(uartctrl, s);
  }
}
/**
 * @Author: zhang
 * @Description: Get the attribute of the each segment
 */
uartSeg* getinformation(void* uartctrl)
{
  int val[UART_ADDR_SIZE];
  uint32_t addr = 0;
  uint32_t segsize = 0;
  for(int i = 0; i < UART_ADDR_SIZE; i++) {
    val[i] = (int)uart_getc(uartctrl);
    val[i] = val[i] << ((UART_ADDR_SIZE - i - 1) * 4);
    addr = addr | val[i];
  }
  for(int i = 0; i < UART_ADDR_SIZE; i++) {
    val[i] = (int)uart_getc(uartctrl);
    val[i] = val[i] << ((UART_ADDR_SIZE - i - 1) * 4);
    segsize = segsize | val[i];
  }
  uartSeg s;
  s.addr = addr;
  s.segsize = segsize;
  return &s;
}
/**
 * @Author: zhang
 * @Description: Get the data of the each segment
 */
void getdata(void* uartctrl, uartSeg* s)
{
  uint32_t* dst_bytes = (uint32_t*) s->addr;
  uint32_t size = s->segsize;
  for(unsigned int i = 0; i < size; i++){
      *dst_bytes = uart_getc(uartctrl);
      dst_bytes++;
  }
}


