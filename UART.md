UART传输：

传输模式：

①以一个起始帧作为UART读入数据的起点

②读入可执行文件的段的数量

③以一个起始帧作为每一个段的开始

④读入段的大小和地址

⑥依据不同的地址和大小读入数据

相关数据结构及常量：

①UART_CTRL_ADDR：UART地址 0x60000000

②UART_REG_RXFIFO ：UART读缓冲区偏移量 0x00

③UART_START_FRAME：UART起始帧 0xFF

④UART_ADDR_SIZE：目的地址长度（字节）4

⑤uartPage：数据结构定义可执行文件

⑥uartSeg：数据结构定义段

相关函数：

①uartRead：读数据总流程

②getinformation：读每一个段的属性（地址、大小）

③createUartPage：读段的数量

④getdata：读段的数据