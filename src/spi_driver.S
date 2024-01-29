
.global spi_read
.section .text
spi_read:

    /* write the 0x70*/
    lui a0, 0x61080
    addi a0, a0, -16
    li a1, 0x70
    sw a1, 0(a0)

    lui a0, 0x61080
    addi a0, a0, -16
    li a1, 0x0  /* the command is zero */
    sw a1, 0(a0)

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    /* write the 0x6 */
    lui a0, 0x61080
    addi a0, a0, -16
    li a1, 0x6
    sw a1, 0(a0)

    lui a0, 0x61080
    addi a0, a0, -16
    li a1, 0x0  /* the command is zero */
    sw a1, 0(a0)

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    /* write the 0x200 */
    lui a0, 0x61080
    addi a0, a0, -4
    li a1, 0x200
    sw a1, 0(a0)
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    /* write 0x1 */
    lui a0, 0x61080
    addi a0, a0, -16
    li a1, 0x1
    sw a1, 0(a0)

    /* write 0x0 */
    lui a0, 0x61080
    addi a0, a0, -4
    li a1, 0x0
    sw a1, 0(a0)

    li t1,15000
    li a1,0
    j _init

_init:
    nop
    nop
    nop
    addi a1, a1,1
    beq a1,t1,_reset
    j _init
    /* initial flash end */

_reset:
    li a0,0
    li t2,0
    li t1,0
    li a1,1000

    lui a0, 0x61080
    addi a0, a0, -4
    li a1, 0x0
    sw a1, 0(a0)

    li a1,1000
    j _read

_read:
    lui a0, 0x61000
    add a0,a0,t1
    addi t1,t1,4
    lw t2, 0(a0)
    beq a1,t1,_stop /* exit the program */
    j _read

_stop:
    j _stop
