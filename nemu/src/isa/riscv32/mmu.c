#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>

paddr_t isa_mmu_translate(vaddr_t addr, int type, int len) {
  return MEM_RET_FAIL;
}

int isa_vaddr_check(vaddr_t vaddr, int type, int len) {
  if (cpu.satp.detail.MODE) return MEM_RET_NEED_TRANSLATE;
  else return MEM_RET_OK;
}


word_t vaddr_mmu_read(vaddr_t addr, int len, int type) {
  // paddr_t pg_base = isa_mmu_translate(addr, type, len);
  // if (result is OK) {
    // paddr_t paddr = ???;
    // return paddr_read(paddr, len);
  // } else if (result is CROSS_PAGE) {
    // return vaddr_read_cross_page(addr, type, len);
  // }
  return 0;
}

void vaddr_mmu_write(vaddr_t addr, word_t data, int len) {

}
