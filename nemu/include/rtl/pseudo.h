#ifndef __RTL_PSEUDO_H__
#define __RTL_PSEUDO_H__

#ifndef __RTL_RTL_H__
#error "Should be only included by <rtl/rtl.h>"
#endif

/* RTL pseudo instructions */

// load

static inline def_rtl(li, rtlreg_t* dest, const rtlreg_t imm) {
  rtl_addi(s, dest, rz, imm);
}

static inline def_rtl(mv, rtlreg_t* dest, const rtlreg_t *src1) {
  if (dest != src1) rtl_add(s, dest, src1, rz);
}

// GH own changes

static inline def_rtl(not, rtlreg_t *dest, const rtlreg_t* src1) {
  *dest = ~ *src1;
}

static inline def_rtl(neg, rtlreg_t *dest, const rtlreg_t* src1) {
  *dest = - *src1;
}

static inline def_rtl(sext, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- signext(src1[(width * 4 - 1) .. 0])
  assert(width > 0);
  uint32_t up_fill;
  switch (width) {
    case 1 : up_fill = 0xfffffff0; break;
    case 2 : up_fill = 0xffffff00; break;
    case 3 : up_fill = 0xfffff000; break;
    case 4 : up_fill = 0xffff0000; break;
    case 5 : up_fill = 0xfff00000; break;
    case 6 : up_fill = 0xff000000; break;
    case 7 : up_fill = 0xf0000000; break;
    default : assert(0);
  }
  *dest = (*src1 & (1 << (width * 4 - 1)) ? *src1 | up_fill : *src1);
}

static inline def_rtl(zext, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- zeroext(src1[(width * 4 - 1) .. 0])
  *dest = *src1; 
}

static inline def_rtl(msb, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- src1[width * 4 - 1]
  *dest = *src1 & (1 << (width * 4 - 1)); 
}


#endif
