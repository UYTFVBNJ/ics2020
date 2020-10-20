// Unconditional Jumps

static inline def_EHelper(jal) {
  rtl_li(s, s0, id_src1->imm);
  rtl_sext(s, s0, s0, 20);
  rtl_add(s, s0, &cpu.pc, s0);
  rtl_jr(s, s0);
  rtl_li(s, ddest, s->seq_pc);

  print_asm_template2(jal);
}

static inline def_EHelper(jalr) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 12);
  rtl_add(s, s0, dsrc1, s0);
  rtl_andi(s, s0, s0, 0xfffffffe);
  rtl_jr(s, s0);
  rtl_li(s, ddest, s->seq_pc);

  print_asm_template3(jalr);
}

// Conditional Branches

static inline def_EHelper(beq) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 13);
  rtl_add(s, s0, &cpu.pc, s0);
  if (interpret_relop(RELOP_EQ, *dsrc1, *ddest)) {
    rtl_jr(s, s0);
    rtl_li(s, ddest, s->seq_pc);
  }

  print_asm_template3(beq);
}

static inline def_EHelper(bne) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 13);
  rtl_add(s, s0, &cpu.pc, s0);
  if (interpret_relop(RELOP_NE, *dsrc1, *ddest)) {
    rtl_jr(s, s0);
    // rtl_li(s, ddest, s->seq_pc);
  }

  print_asm_template3(bne);
}

static inline def_EHelper(blt) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 13);
  rtl_add(s, s0, &cpu.pc, s0);
  if (interpret_relop(RELOP_LT, *dsrc1, *ddest)) {
    rtl_jr(s, s0);
    // rtl_li(s, ddest, s->seq_pc);
  }

  print_asm_template3(blt);
}

static inline def_EHelper(bltu) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 13);
  rtl_add(s, s0, &cpu.pc, s0);
  if (interpret_relop(RELOP_LTU, *dsrc1, *ddest)) {
    rtl_jr(s, s0);
    // rtl_li(s, ddest, s->seq_pc);
  }

  print_asm_template3(bltu);
}

static inline def_EHelper(bge) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 13);
  rtl_add(s, s0, &cpu.pc, s0);
  if (interpret_relop(RELOP_GE, *dsrc1, *ddest)) {
    rtl_jr(s, s0);
    // rtl_li(s, ddest, s->seq_pc);
  }

  print_asm_template3(bge);
}

static inline def_EHelper(bgeu) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 13);
  rtl_add(s, s0, &cpu.pc, s0);
  if (interpret_relop(RELOP_GEU, *dsrc1, *ddest)) {
    rtl_jr(s, s0);
    // rtl_li(s, ddest, s->seq_pc);
  }

  print_asm_template3(bgeu);
}

static inline def_EHelper(CB) {
  switch(s->isa.instr.b.funct3) {
    case 0 : exec_beq(s);    break;
    case 1 : exec_bne(s);    break;
    case 4 : exec_blt(s);    break;
    case 5 : exec_bge(s);    break;
    case 6 : exec_bltu(s);   break;
    case 7 : exec_bgeu(s);   break;
    default : assert(0); break;
  }
}