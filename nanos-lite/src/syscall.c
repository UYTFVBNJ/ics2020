#include <common.h>
#include "syscall.h"

inline void SYS_exit_handler(Context *c) {
  halt(0);
}

inline void SYS_yield_handler(Context *c) {
  yield();

  c->epc += 4;
}

int fs_open(const char *pathname, int flags, int mode);
inline void SYS_open_handler(Context *c) {
  const char * path = (char*)c->GPR2;
  int flags = c->GPR3;
  int mode = c->GPR4;

  c->GPRx = fs_open(path, flags, mode);

  c->epc += 4;
}

inline void SYS_write_handler(Context *c) {
  int fd = c->GPR2;
  char * buf = (char*)c->GPR3;
  size_t count = c->GPR4;
  size_t i = 0;

  if (fd == 1 || fd == 2) {
    for (i = 0; i < count; i ++) putch(buf[i]);
    c->GPRx = i;
  } else {
    c->GPRx = -1;
  }

  c->epc += 4;
}

inline void SYS_brk_handler(Context *c) {
  c->GPRx = 0;

  c->epc += 4;
}

#define SYS_handle(x) case SYS_ ## x: SYS_ ## x ## _handler(c);     break

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    SYS_handle(exit);
    SYS_handle(yield);
    SYS_handle(open);
    SYS_handle(write);
    SYS_handle(brk);

    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}