#include <isa.h>
#include "local-include/reg.h"

const char *regsl[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  puts("REG INFO: ");
  for (int i=0;i<4;i++) {
	for (int j=0;j<8;j++)	printf("$%11s$",regsl[8*i+j]); 
	  puts("");
	  for (int j=0;j<8;j++)	printf("|   %08x|",reg_l(8*i+j)); 
	  puts("");
	  for (int j=0;j<8;j++)	printf("|%11u|",reg_l(8*i+j)); 
	  puts("");

	}
  printf("pc: %08x\n",cpu.pc);
}

word_t isa_reg_str2val(const char *s, bool *success) {
  for (int i = 0; i < 32; i ++) if (strcmp(regsl[i], s) == 0) return reg_l(i);
  *success = 0;
  return 0;
}
