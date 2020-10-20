// Multiplication Operations

static inline def_EHelper(mul) {
  rtl_muls_lo(s, ddest, dsrc1, dsrc2);

  print_asm_template3(mul);
}

static inline def_EHelper(mulh) {
  rtl_muls_hi(s, ddest, dsrc1, dsrc2);

  print_asm_template3(mulh);
}

static inline def_EHelper(mulhsu) {
  rtl_mul_lo(s, ddest, dsrc1, dsrc2);

  print_asm_template3(mulhsu);
}

static inline def_EHelper(mulhu) {
  rtl_mul_hi(s, ddest, dsrc1, dsrc2);

  print_asm_template3(mulhu);
}


// Division Operations

static inline def_EHelper(div) {
  rtl_divs_q(s, ddest, dsrc1, dsrc2);

  print_asm_template3(div);
}

static inline def_EHelper(divu) {
  rtl_div_q(s, ddest, dsrc1, dsrc2);

  print_asm_template3(divu);
}

static inline def_EHelper(rem) {
  rtl_divs_r(s, ddest, dsrc1, dsrc2);

  print_asm_template3(rem);
}

static inline def_EHelper(remu) {
  rtl_div_r(s, ddest, dsrc1, dsrc2);

  print_asm_template3(remu);
}

static inline def_EHelper(IMD) {
  switch(s->isa.instr.i.funct3) {
    case 0 : exec_mul(s);       break;
    case 1 : exec_mulh(s);      break;
    case 2 : exec_mulhsu(s);    break;
    case 3 : exec_mulhu(s);     break;
    case 4 : exec_div(s);       break;
    case 5 : exec_divu(s);      break;
    case 6 : exec_rem(s);       break;
    case 7 : exec_remu(s);      break;
    default : assert(0); break;
  }
}
