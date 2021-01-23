#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>

paddr_t isa_mmu_translate(vaddr_t addr, int type, int len) {
  union VA va = (union VA)addr;
  paddr_t a = cpu.satp.detail.PPN * PAGE_SIZE;

  printf("0mmu: %x %d\n", cpu.pc, len);
  printf("1mmu: %x\n", a + va.detail.VPN1 * PTE_SIZE);
  union PTE pte = (union PTE)paddr_read(a + va.detail.VPN1 * PTE_SIZE, 4);
  printf("mmu:\n");

  assert(pte.detail.V);

  assert(!(pte.detail.R || pte.detail.W || pte.detail.X));

  a = (pte.val >> 10) * PAGE_SIZE;

  printf("2mmu: %x\n", a + va.detail.VPN0 * PTE_SIZE);
  pte = (union PTE)paddr_read(a + va.detail.VPN0 * PTE_SIZE, 4);
  assert(pte.detail.V);
  printf("mmu:\n");

  // A leaf PTE has been found.

  // prot is not enable now
  // if (type == MEM_TYPE_READ   && !pte.detail.R) assert(0);
  // if (type == MEM_TYPE_WRITE  && !pte.detail.W) assert(0);
  // if (type == MEM_TYPE_IFETCH && !pte.detail.X) assert(0);

  // A, D ignored

  union PA pa;

  pa.detail.page_offest = va.detail.page_offest;
  pa.detail.PPN0 = pte.detail.PPN0;
  pa.detail.PPN1 = pte.detail.PPN1;

  return pa.val;
}

int isa_vaddr_check(vaddr_t vaddr, int type, int len) {
  if (cpu.satp.detail.MODE) return MEM_RET_NEED_TRANSLATE;
  else return MEM_RET_OK;
}


word_t vaddr_mmu_read(vaddr_t addr, int len, int type) {
  paddr_t paddr = isa_mmu_translate(addr, type, len);
  return paddr_read(paddr, len);
}

void vaddr_mmu_write(vaddr_t addr, word_t data, int len) {
  paddr_t paddr = isa_mmu_translate(addr, MEM_TYPE_WRITE, len);
  paddr_write(paddr, data, len);
}
