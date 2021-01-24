#include <memory.h>
#include <proc.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void * ptr = pf;
  pf = pf + nr_page * PGSIZE;
  // printf("new_page pf %p\n", pf);

  return ptr;
}

static inline void* pg_alloc(int n) { // 我们保证AM通过回调函数调用pg_alloc()时申请的空间总是页面大小的整数倍
  void * ptr = new_page(n / PGSIZE);

  memset(ptr, 0, n);

  return ptr;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  printf("mm_brk: %p\n", brk);
  if (current->max_brk == 0) {
    current->max_brk = brk;
    return 0;
  }

  while ((current->max_brk & ~(PGSIZE - 1)) != (brk & ~(PGSIZE - 1))) {
    uint32_t ptr = (uint32_t)new_page(1);
    map(&current->as, (void*)current->max_brk, (void*)ptr, 0); // native set RWX
    current->max_brk += PGSIZE;
  }
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);

#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
