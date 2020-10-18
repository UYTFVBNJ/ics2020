// Integer Register-Immediate Instructions

static inline def_EHelper(addi) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 3);
  rtl_add(s, ddest, dsrc1, s0);

  print_asm_template3(addi);
}

static inline def_EHelper(slti) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 3);
  rtl_setrelop(s, RELOP_LT, ddest, dsrc1, s0);

  print_asm_template3(slti);
}

static inline def_EHelper(sltiu) {
  rtl_li(s, s0, id_src2->imm);
  rtl_setrelop(s, RELOP_LTU, ddest, dsrc1, s0);

  print_asm_template3(sltiu);
}

static inline def_EHelper(andi) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 3);
  rtl_and(s, ddest, dsrc1, s0);

  print_asm_template3(andi);
}

static inline def_EHelper(ori) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 3);
  rtl_or(s, ddest, dsrc1, s0);

  print_asm_template3(ori);
}

static inline def_EHelper(xori) {
  rtl_li(s, s0, id_src2->imm);
  rtl_sext(s, s0, s0, 3);
  rtl_xor(s, ddest, dsrc1, s0);

  print_asm_template3(xori);
}

static inline def_EHelper(slli) {
  rtl_shli(s, ddest, dsrc1, id_src2->imm);

  print_asm_template3(slli);
}

static inline def_EHelper(srli_srai) {
  switch (id_src2->imm & (1 << 11)) {
    case         0 : 
      rtl_shri(s, ddest, dsrc1, id_src2->imm & 0x1f);
       
      print_asm_template3(srli);
      break;
    case (1 << 11) : 
      rtl_sari(s, ddest, dsrc1, id_src2->imm & 0x1f); 
  
      print_asm_template3(srai);
      break;
  }

}

static inline def_EHelper(IRII) {
  switch(s->isa.instr.i.funct3) {
    case 0 : exec_addi(s);      break;
    case 1 : exec_slli(s);      break;
    case 2 : exec_slti(s);      break;
    case 3 : exec_sltiu(s);     break;
    case 4 : exec_xori(s);      break;
    case 5 : exec_srli_srai(s); break;
    case 6 : exec_ori(s);       break;
    case 7 : exec_andi(s);      break;
    default : break;
  }
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
/*
static inline def_EHelper(IRRO) {
  switch(s->isa.instr.r.funct3) {
    case 0 : exec_addi(s);      break;
    case 1 : exec_slli(s);      break;
    case 2 : exec_slti(s);      break;
    case 3 : exec_sltiu(s);     break;
    case 4 : exec_xori(s);      break;
    case 5 : exec_srli_srai(s); break;
    case 6 : exec_ori(s);       break;
    case 7 : exec_andi(s);      break;
    default : break;
  }
}*/