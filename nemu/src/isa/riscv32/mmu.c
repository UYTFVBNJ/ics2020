#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>

paddr_t isa_mmu_translate(vaddr_t addr, int type, int len) {
  return MEM_RET_FAIL;
}

int isa_vaddr_check(vaddr_t vaddr, int type, int len) {
  if (cpu.csr[4]._32 & 0x80000000) return MEM_RET_NEED_TRANSLATE;
  else return MEM_RET_OK;
}


word_t vaddr_mmu_read(vaddr_t addr, int len, int type) {
  return 0;
}

void vaddr_mmu_write(vaddr_t addr, word_t data, int len) {

}
