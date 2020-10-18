// Integer Register-Immediate Instructions
static inline def_EHelper(addi) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 3);
  rtl_add(s, ddest, s0, ddest);
}

static inline def_EHelper(slti) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 3);
  rtl_setrelop(s, RELOP_LT, ddest, dsrc1, s0);
}

static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}

static inline def_EHelper(auipc) {
  rtl_li(s, ddest, s->seq_pc);
  rtl_addi(s, ddest, rz, id_src1->imm);
  
  print_asm_template2(auipc);
}

// Integer Register-Register Operations

