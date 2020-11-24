/*static inline def_EHelper(CRSRRW) {
  switch(s->isa.instr.i.simm11_0) {
    case 0x100 : exec_(s);    break; // sstatus
    case 0x141 : exec_(s);    break; // spec
    case 0x142 : exec_(s);    break; // scause
    default : assert(0); break;
  }


}

static inline def_EHelper(CRS) {
  switch(s->isa.instr.i.funct3) {
    case 0 : exec_(s);    break;
    case 1 : exec_(s);    break;
    case 4 : exec_(s);    break;
    case 5 : exec_(s);    break;
    case 6 : exec_(s);   break;
    case 7 : exec_(s);   break;
    default : assert(0); break;
  }
}
*/