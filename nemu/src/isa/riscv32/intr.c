#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  rtl_li(s, &cpu.csr[2]._32, epc); // sepc
  printf("NO: %d\n", NO);
  rtl_li(s, &cpu.csr[3]._32, NO);  // scause
  rtl_jr(s, &cpu.csr[1]._32);      // stvec

}

void query_intr(DecodeExecState *s) {
}
