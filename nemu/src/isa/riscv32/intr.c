#include <cpu/exec.h>
#include "local-include/rtl.h"

#define IRQ_TIMER 0x80000005  // for riscv32

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  cpu.sstatus.detail.SPIE = cpu.sstatus.detail.SIE; // sstatus
  cpu.sstatus.detail.SIE = 0; // sstatus
  rtl_li(s, &cpu.sepc.val, epc); // sepc
  rtl_li(s, &cpu.scause.val, NO);  // scause
  rtl_jr(s, &cpu.stvec.val);      // stvec

}

void query_intr(DecodeExecState *s) {
  if (cpu.INTR) {
    cpu.INTR = false;
    raise_intr(s, IRQ_TIMER, s->seq_pc - 4); // the inst most recently exe ed
    update_pc(s);
  }
}
