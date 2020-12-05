#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  union {
    uintptr_t gpr[32];
    void *pdir;
  };
  uintptr_t epc, status, cause;
};

#define GPR1 gpr[17] // a7
#define GPR2 gpr[0]
#define GPR3 gpr[0]
#define GPR4 gpr[0]
#define GPRx gpr[0]

#endif
