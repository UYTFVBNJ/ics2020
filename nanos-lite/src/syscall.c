#include <common.h>
#include <proc.h>
#include "syscall.h"

void naive_uload(PCB *, const char *);
inline Context* SYS_execve_handler(Context *c) {

  const char * pathname = (char *)c->GPR2;
  // char * const argv = (char *)c->GPR3;
  // char * const envp = (char *)c->GPR4;

  naive_uload(NULL, pathname);

  c->GPRx = 0;

  c->epc += 4;
  return c;
}

inline Context* SYS_exit_handler(Context *c) {
  c->GPR2 = (uintptr_t)&"/bin/nterm";
  SYS_execve_handler(c);
  return c;
}

Context* schedule(Context *prev);
inline Context* SYS_yield_handler(Context *c) {
  printf("yield\n");
  // yield();
  c = schedule(c);

  c->epc += 4;
  return c;
}

int fs_open(const char *pathname, int flags, int mode);

inline Context* SYS_open_handler(Context *c) {
  const char * path = (char*)c->GPR2;
  int flags = c->GPR3;
  int mode = c->GPR4;

  c->GPRx = fs_open(path, flags, mode);

  c->epc += 4;
  return c;
}


size_t fs_read(int fd, void *buf, size_t len);

inline Context* SYS_read_handler(Context *c) {
  int fd = c->GPR2;
  void * buf = (void*)c->GPR3;
  size_t len = c->GPR4;

  c->GPRx = fs_read(fd, buf, len);

  c->epc += 4;
  return c;
}

size_t fs_write(int fd, const void *buf, size_t len);

inline Context* SYS_write_handler(Context *c) {
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
  return c;
}

int fs_close(int fd);

inline Context* SYS_close_handler(Context *c) {
  int fd = c->GPR2;

  c->GPRx = fs_close(fd);

  c->epc += 4;
  return c;
}

size_t fs_lseek(int fd, size_t offset, int whence);

inline Context* SYS_lseek_handler(Context *c) {
  int fd = c->GPR2;
  size_t offset =  c->GPR3;
  int whence = c->GPR4;

  c->GPRx = fs_lseek(fd, offset, whence);

  c->epc += 4;
  return c;
}

inline Context* SYS_brk_handler(Context *c) {
  printf("brk\n");
  c->GPRx = 0;

  c->epc += 4;
  return c;
}



struct timeval {
  __time_t tv_sec;		/* Seconds.  */
  __suseconds_t tv_usec;	/* Microseconds.  */
} * tv;
inline Context* SYS_gettimeofday_handler(Context *c) {

  AM_TIMER_UPTIME_T rtc = io_read(AM_TIMER_UPTIME);

  tv = (struct timeval *)c->GPR2;

  tv->tv_sec = rtc.us / 1000000;
  tv->tv_usec = rtc.us % 1000000;

  c->GPRx = 0;

  c->epc += 4;
  return c;
}

#define SYS_handle(x) case SYS_ ## x: return SYS_ ## x ## _handler(c);     break

Context* do_syscall(Context *c) {
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

    SYS_handle(execve);
  

    SYS_handle(gettimeofday);

    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return c;
}