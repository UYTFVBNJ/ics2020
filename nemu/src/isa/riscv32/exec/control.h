static inline def_EHelper(jal) {
  rtl_li(s, ddest, s->seq_pc);
  rtl_addi(s, ddest, rz, id_src1->imm);
  
  print_asm_template2(auipc);
}