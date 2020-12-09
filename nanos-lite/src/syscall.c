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


size_t fs_read(int fd, void *buf, size_t len);

inline void SYS_read_handler(Context *c) {
  int fd = c->GPR2;
  void * buf = (void*)c->GPR3;
  size_t len = c->GPR4;

  c->GPRx = fs_read(fd, buf, len);

  c->epc += 4;
}

size_t fs_write(int fd, const void *buf, size_t len);

inline void SYS_write_handler(Context *c) {
  int fd = c->GPR2;
  const char * buf = (char*)c->GPR3;
  size_t count = c->GPR4;
  size_t i = 0;

  if (fd == 1 || fd == 2) {
    for (i = 0; i < count; i ++) putch(buf[i]);
    c->GPRx = i;
  } else {
    c->GPRx = fs_write(fd, buf, count);
  }

  c->epc += 4;
}

int fs_close(int fd);

inline void SYS_close_handler(Context *c) {
  int fd = c->GPR2;

  c->GPRx = fs_close(fd);

  c->epc += 4;
}

size_t fs_lseek(int fd, size_t offset, int whence);

inline void SYS_lseek_handler(Context *c) {
  int fd = c->GPR2;
  size_t offset =  c->GPR3;
  int whence = c->GPR4;

  c->GPRx = fs_lseek(fd, offset, whence);

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
    SYS_handle(read);
    SYS_handle(write);
    SYS_handle(close);
    SYS_handle(lseek);
    SYS_handle(brk);

    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}