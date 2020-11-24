static inline def_EHelper(CRSRRW) {
  rtl_mv(s, ddest, dsrc2);
  rtl_mv(s, dsrc2, dsrc1);

  print_asm_template3(crsrrw);
}

static inline def_EHelper(CSR) {
  switch(id_src2->imm) {
    case 0x100 : id_src2->preg = &cpu.csr[0]._32;    break; // sstatus
    case 0x105 : id_src2->preg = &cpu.csr[1]._32;    break; // stvec
    case 0x141 : id_src2->preg = &cpu.csr[2]._32;    break; // spec
    case 0x142 : id_src2->preg = &cpu.csr[3]._32;    break; // scause
    default : assert(0); break;
  }

  switch(s->isa.instr.i.funct3) {
    // case 0 : exec_(s);    break;
    case 1 : exec_CRSRRW(s);    break;
    // case 4 : exec_(s);    break;
    // case 5 : exec_(s);    break;
    // case 6 : exec_(s);   break;
    // case 7 : exec_(s);   break;
    default : assert(0); break;
  }
}