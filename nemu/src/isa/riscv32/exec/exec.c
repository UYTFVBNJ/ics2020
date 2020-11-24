#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width) {
  if (width != 0) s->width = width;
}

// exe definitions

static inline def_EHelper(load) {
  switch (s->isa.instr.i.funct3) {
    EXW  (0b000, lds, 1)
    EXW  (0b001, lds, 2)
    EXW  (0b010, ld, 4)
    EXW  (0b100, ld, 1)
    EXW  (0b101, ld, 2)

    default: exec_inv(s);
  }
}

static inline def_EHelper(store) {
  switch (s->isa.instr.s.funct3) {
    EXW  (0b000, st, 1)
    EXW  (0b001, st, 2)
    EXW  (0b010, st, 4)
    default: exec_inv(s);
  }
}

// exe definitions 
void difftest_skip_dut(int, int);

static inline void fecth_decode_exec(DecodeExecState *s) {
  s->isa.instr.val = instr_fetch(&s->seq_pc, 4);
  assert(s->isa.instr.i.opcode1_0 == 0x3);
  
#ifdef DIFF_TEST
  if (s->isa.instr.i.opcode6_2 == 0b11001) difftest_skip_dut(1, 2);
#endif

  switch (s->isa.instr.i.opcode6_2) {

    // ldst
    IDEX (0b00000, I, load)
    IDEX (0b01000, S, store)

    // compute

    // Integer Register-Immediate Instructions
    IDEX (0b00100, I, IRII)

    IDEX (0b01101, U, lui)
    IDEX (0b00101, U, auipc)

    // Integer Register-Register Operations
    IDEX (0b01100, R, IRRO_IMD);

    // control
    IDEX (0b11011, J, jal);
    IDEX (0b11001, I, jalr);

    IDEX (0b11000, B, CB);
    
    // system
    // IDEX (0b11100, I, CSR);

    EX   (0b11010, nemu_trap)

    default: exec_inv(s);
  }
}

static inline void reset_zero() {
  reg_l(0) = 0;
}

vaddr_t isa_exec_once() {
  DecodeExecState s;
  s.is_jmp = 0;
  s.seq_pc = cpu.pc;

  fecth_decode_exec(&s);
  update_pc(&s);

  reset_zero();

  return s.seq_pc;
}
