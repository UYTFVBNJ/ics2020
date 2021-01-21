#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>

union PTE PTE_read(paddr_t addr);
void PTE_write(paddr_t addr, word_t data);

paddr_t isa_mmu_translate(vaddr_t addr, int type, int len) {
  union VA va = (union VA)addr;
  paddr_t a = cpu.satp.detail.PPN;
  union PTE pte = PTE_read(a + va.detail.VPN1 * PTE_SIZE);

  if (pte.detail.V == 0) assert(0);
  else {
    assert(!(pte.detail.R || pte.detail.W || pte.detail.X));
    a = (pte.val >> 10) * PAGE_SIZE;

    pte = PTE_read(a + va.detail.VPN0 * PTE_SIZE);

    // A leaf PTE has been found.

    if (type == MEM_TYPE_READ   && !pte.detail.R) assert(0);
    if (type == MEM_TYPE_WRITE  && !pte.detail.W) assert(0);
    if (type == MEM_TYPE_IFETCH && !pte.detail.X) assert(0);

    // A, D ignored

    union PA pa;

    pa.detail.page_offest = va.detail.page_offest;
    pa.detail.PPN0 = pte.detail.PPN0;
    pa.detail.PPN1 = pte.detail.PPN1;

    return pa.val;
  }
}

int isa_vaddr_check(vaddr_t vaddr, int type, int len) {
  if (cpu.satp.detail.MODE) return MEM_RET_NEED_TRANSLATE;
  else return MEM_RET_OK;
}


word_t vaddr_mmu_read(vaddr_t addr, int len, int type) {
  // paddr_t pg_base = isa_mmu_translate(addr, type, len);

  paddr_t paddr = 0;
  return paddr_read(paddr, len);
}

void vaddr_mmu_write(vaddr_t addr, word_t data, int len) {

}
