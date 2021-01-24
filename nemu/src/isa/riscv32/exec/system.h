void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t epc);

static inline def_EHelper(CSRRW) {
  if (id_dest->reg != 0) rtl_mv(s, ddest, dsrc2);
  rtl_mv(s, dsrc2, dsrc1);

  print_asm_template3(csrrw);
}

static inline def_EHelper(CSRRS) {
  rtl_mv(s, ddest, dsrc2);
  if (id_src1->reg != 0) rtl_or(s, dsrc2, dsrc2, dsrc1);

  print_asm_template3(csrrs);
}

static inline def_EHelper(CSRRC) {
  rtl_mv(s, ddest, dsrc2);
  rtl_sub(s, s0, rz, dsrc1);
  if (id_src1->reg != 0) rtl_and(s, dsrc2, dsrc2, s0);

  print_asm_template3(csrrc);
}

static inline def_EHelper(CSRRWI) {
  if (id_dest->reg != 0) rtl_mv(s, ddest, dsrc2);
  rtl_li(s, dsrc2, id_src1->reg);

  print_asm_template3(csrrwi);
}

static inline def_EHelper(CSRRSI) {
  rtl_mv(s, ddest, dsrc2);
  if (id_src1->reg != 0) rtl_ori(s, dsrc2, dsrc2, id_src1->reg);

  print_asm_template3(csrrsi);
}

static inline def_EHelper(CSRRCI) {
  rtl_mv(s, ddest, dsrc2);
  rtl_subi(s, s0, rz, id_src1->reg);
  if (id_src1->reg != 0) rtl_and(s, dsrc2, dsrc2, s0);

  print_asm_template3(csrrci);
}

static inline def_EHelper(SRET) {
  rtl_jr(s, &cpu.sepc.val);

  print_asm_template3(sret);
}

static inline def_EHelper(ECALL) {
  raise_intr(s, 1, cpu.pc);

  print_asm_template3(ecall);
}

static inline def_EHelper(CSR) {
  switch(id_src2->imm) {
    case 0x000 : if (s->isa.instr.i.funct3 == 0) exec_ECALL(s); return ; // ECALL
    case 0x102 : if (s->isa.instr.i.funct3 == 0) exec_SRET(s);  return ; // SRET

    case 0x100 : id_src2->preg = &cpu.sstatus.val;    break; // sstatus
    case 0x105 : id_src2->preg = &cpu.stvec.val;    break; // stvec
    case 0x141 : id_src2->preg = &cpu.sepc.val;    break; // sepc
    case 0x142 : id_src2->preg = &cpu.scause.val;    break; // scause
    case 0x180 : id_src2->preg = &cpu.satp.val;    break; // satp

    default : exec_inv(s); assert(0); break;
  }

  switch(s->isa.instr.i.funct3) {
    case 1 : exec_CSRRW(s);      break;
    case 2 : exec_CSRRS(s);      break;
    // case 4 : exec_(s);    break;
    // case 5 : exec_(s);    break;
    // case 6 : exec_(s);   break;
    // case 7 : exec_(s);   break;
    default : exec_inv(s); assert(0); break;
  }
}