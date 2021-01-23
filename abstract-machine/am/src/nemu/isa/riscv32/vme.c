#include <am.h>
#include <nemu.h>
#include <klib.h>

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

static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static inline void set_satp(void *pdir) {
  asm volatile("csrw satp, %0" : : "r"(0x80000000 | ((uintptr_t)pdir >> 12)));
}

static inline uintptr_t get_satp() {
  uintptr_t satp;
  asm volatile("csrr %0, satp" : "=r"(satp));
  return satp << 12;
}

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  kas.ptr = pgalloc_f(PGSIZE);

  int i;
  for (i = 0; i < LENGTH(segments); i ++) {
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      map(&kas, va, va, 0);
    }
  }

  set_satp(kas.ptr);
  vme_enable = 1;

  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE)); // PTE again
  as->ptr = updir;
  as->area = USER_SPACE;
  as->pgsize = PGSIZE;
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->pdir != NULL) {
    set_satp(c->pdir);
  }
}

void map(AddrSpace *as, void *va_, void *pa_, int prot) {
  // if (prot == 0) panic("Not implemented");
  printf("map: va %p to pa %p with prot %d\n", va_, pa_, prot);

  union VA va = (union VA)(uint32_t)va_;
  union PA pa = (union PA)(uint32_t)pa_;
  union PTE * pte = (union PTE *)as->ptr + va.detail.VPN1; // no need to multiply 4
  printf("map: va %p to pa %p with prot %p\n", as->ptr, va.detail.VPN1, pte);

  // finding corresponding PTE
  if (pte->val == 0) {
    pte->val = (uint32_t)pgalloc_usr(PGSIZE);
  }

  pte = (union PTE *)((pte->val >> 10) * PGSIZE) + va.detail.VPN0;

  // filling PTE
  pte->detail.V = 1;

  pte->detail.PPN0 = pa.detail.PPN0;
  pte->detail.PPN1 = pa.detail.PPN1;
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
  // printf("ucontext:\n");
  uint32_t * base = (uint32_t *)kstack.end - (32 + 3);
  // printf("base:%p\n", base);

  for (int i = 0; i < 32; i ++) *(base + i) = 0;

  *(base + 2) = (uint32_t)kstack.end; // sp

  *(base + 32) = 0; // CAUSE
  *(base + 33) = 0; // STATUS
  *(base + 34) = (uint32_t)entry; // EPC

  return (Context*)base;
}
