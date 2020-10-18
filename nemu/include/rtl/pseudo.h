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
  *dest = (*src1 & (1 << (width * 4 - 1)) ? *src1 | upper_fill_1[width] : *src1);
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
