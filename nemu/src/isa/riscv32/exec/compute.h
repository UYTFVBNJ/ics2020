// Integer Register-Immediate Instructions
// static inline def_EHelper(addi) {
// 
// }

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

