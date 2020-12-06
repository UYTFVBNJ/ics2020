#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case 0: SYS_exit(c);      break;
    case 1: SYS_yield(c);     break;
    case 2: SYS_open(c);      break;
    
    case 4: SYS_write(c);     break;

    case 9: SYS_brk(c);       break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}