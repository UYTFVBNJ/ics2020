#ifndef __ISA_RISCV32_H__
#define __ISA_RISCV32_H__

#include <common.h>

// memory
#define riscv32_IMAGE_START 0x100000
#define riscv32_PMEM_BASE 0x80000000

// reg

typedef struct {
  struct {
    rtlreg_t _32;
  } gpr[32];

  struct {
    rtlreg_t _32;
  } csr[4];

  union {
    struct {
      uint32_t PPN : 22;
      uint32_t ASID : 9;
      uint32_t MODE : 1;
    }detail;
    uint32_t val;
  }satp;  

  vaddr_t pc;
} riscv32_CPU_state;

// decode
typedef struct {
  union {
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      int32_t  funct7    : 7;
    } r;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      int32_t  simm11_0  :12;
    } i;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t imm4_0    : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      int32_t  simm11_5  : 7;
    } s;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t imm11_11  : 1;
      uint32_t imm4_1    : 4;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      uint32_t imm10_5   : 6;
      uint32_t simm12_12 : 1;
    } b;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t imm31_12  :20;
    } u;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t imm19_12  : 8;
      uint32_t imm11_11  : 1;
      uint32_t imm10_1   :10;
      uint32_t simm20_20 : 1;
    } j;
    uint32_t val;
  } instr;
} riscv32_ISADecodeInfo;

paddr_t isa_mmu_translate(vaddr_t vaddr, int type, int len);
// #define isa_vaddr_check(vaddr, type, len) (MEM_RET_OK)
int isa_vaddr_check(vaddr_t vaddr, int type, int len);
#define riscv32_has_mem_exception() (false)

union PTE{
  struct {
      uint32_t V : 1;
      uint32_t R : 1;
      uint32_t W : 1;
      uint32_t X : 1;

      uint32_t U : 1;
      uint32_t G : 1;
      uint32_t A : 1;
      uint32_t D : 1;

      uint32_t RSW : 2;

      uint32_t PPN0 : 10;
      uint32_t PPN1 : 12;
  }detail;
  uint32_t val;
};
#define PTE_SIZE 4

union VA{
  struct {
      uint32_t page_offest : 12;

      uint32_t VPN0 : 10;
      uint32_t VPN1 : 10;
  }detail;
  uint32_t val;
};

union PA{
  struct {
      uint32_t page_offest : 12;

      uint32_t PPN0 : 10;
      uint32_t PPN1 : 10; // might be changed to 12
  }detail;
  uint32_t val;
};

#endif
