#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  union {
    uintptr_t gpr[32];
    void *pdir;
  };
  uintptr_t cause, status, epc;
};

#define GPR1 gpr[17] // a7
#define GPR2 gpr[10] // a0
#define GPR3 gpr[11] // a1
#define GPR4 gpr[12] // a2
#define GPRx gpr[10] // a0

#endif
