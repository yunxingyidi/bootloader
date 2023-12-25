#include "platform.h"

  .section .text.init
  .globl _prog_start
_prog_start:
  .globl _start
_start:
  la t0, trap_entry
  csrw mtvec, t0
  call main
1:
  j 1b


trap_entry:
  call handle_trap