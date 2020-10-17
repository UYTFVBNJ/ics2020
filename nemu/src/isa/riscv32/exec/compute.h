// Integer Register-Immediate Instructions

static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}

static inline def_EHelper(auipc) {
  // rtl_addi(s, ddest, id_src1->imm);
  print_asm_template2(auipc);
}

// Integer Register-Register Operations

