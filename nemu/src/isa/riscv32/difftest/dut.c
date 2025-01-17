#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  for (int i = 0; i < 32; i ++) if (ref_r->gpr[i]._32 != cpu.gpr[i]._32) {
    printf("%d %u %u\n", i, ref_r->gpr[i]._32, cpu.gpr[i]._32);
    printf("%d %x %x\n", i, ref_r->gpr[i]._32, cpu.gpr[i]._32);
    return 0;
  }
  return 1;
}

void isa_difftest_attach() {
}
