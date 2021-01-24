#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

uint32_t min(uint32_t a, uint32_t b) {
  if (a < b) return a;
  return b;
}

void* new_page(size_t nr_page);
static uintptr_t loader_set_pg(PCB *pcb, uint32_t vaddr, uint32_t * last_pg_va, uint32_t * last_pg_pa) {
  if (*last_pg_va != (vaddr & ~(PGSIZE - 1))) {
    *last_pg_pa = (uint32_t)new_page(1);
    printf("setting va %p at pa %p\n", vaddr, *last_pg_pa);
    *last_pg_va = vaddr & ~(PGSIZE - 1);
    map(&pcb->as, (void*)*last_pg_va, (void*)*last_pg_pa, 0); // native set RWX
  }

  uint32_t offset = vaddr & (PGSIZE - 1);
  return *last_pg_pa + offset;
}

static uintptr_t loader(PCB *pcb, const char *filename) {
  printf("loading %s\n", filename);
  int fd = fs_open(filename, 0, 0);
  static Elf_Ehdr eh;
  static Elf_Phdr ph;

  fs_lseek(fd, 0, 0);
  fs_read(fd, &eh, 52);

  //
  // printf("eh: %d\n", eh.e_phoff);
  // printf("eh: %d\n", eh.e_phentsize);
  // printf("eh: %d\n", eh.e_phnum);

  uint32_t last_pg_va = 0;
  uint32_t last_pg_pa = 0;
  for (int i = 0; i < eh.e_phnum; i ++) {
    fs_lseek(fd, eh.e_phoff + i * eh.e_phentsize, 0);
    fs_read(fd, &ph, eh.e_phentsize);

    //
    // printf("ph: %d\n", ph.p_type);
    // printf("ph: %p\n", ph.p_offset);
    // printf("ph: %p\n", ph.p_vaddr);
    // printf("ph: %d\n", ph.p_filesz);
    // printf("ph: %d\n", ph.p_memsz);
    // printf("ph: \n");
    
    if (ph.p_type == 1) {
      fs_lseek(fd, ph.p_offset, 0);

      int done_sz = 0;
      while (done_sz < ph.p_filesz) {
        uint32_t pa = loader_set_pg(pcb, ph.p_vaddr + done_sz, &last_pg_va, &last_pg_pa);

        printf("loading va %p at pa %p\n", ph.p_vaddr + done_sz, pa);

        fs_read(fd, (void(*))pa, min(ph.p_filesz, (uint32_t)PGSIZE));
        done_sz += min(ph.p_filesz, (uint32_t)PGSIZE);
      }


      for (size_t j = ph.p_vaddr + ph.p_filesz; j < ph.p_vaddr + ph.p_memsz; j ++) {
        uint32_t pa = loader_set_pg(pcb, j, &last_pg_va, &last_pg_pa);

        printf("filling va %p at pa %p\n", j, pa);

        *((uint32_t *)pa) = 0;
      }
    }
  }

  pcb->max_brk = last_pg_va;
  printf("loader finished %s\n", filename);
  return eh.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg);

void context_kload(PCB *pcb, void (*entry)(void *), void *arg) {
  Area kstack;
  kstack.start = pcb->stack;
  kstack.end   = pcb->stack + STACK_SIZE;

  pcb->cp = kcontext(kstack , entry, arg);
}

uint32_t ustk_get_va(uint32_t va_base, uint32_t pa_base, uint32_t pa) {
  return va_base - (pa_base - pa);
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry);
void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]) {
  protect(&pcb->as);

  Area kstack;
  kstack.start = pcb->stack;
  kstack.end   = pcb->stack + STACK_SIZE;


  // loading args
  // char * ustk_pt_1 = (char *)heap.end;
  char * ptr = (char *)new_page(8) + 8 * PGSIZE;
  for (int i = 0; i < 8; i ++) {
    map(&pcb->as, pcb->as.area.end - i * PGSIZE - 1, ptr - i * PGSIZE - 1, 0);
  }

  char * ustk_pt_1 = ptr - 1;

  printf("uload placing stack at %p\n", ustk_pt_1);
  
  int ustk_envp_len = 0;
  char * ustk_envp[10];
  if (envp != NULL)
  for (ustk_envp_len = 0; envp[ustk_envp_len] != NULL; ustk_envp_len ++) {
    printf("uload envp[%d] : %s\n", ustk_envp_len, envp[ustk_envp_len]);
    size_t len;

    for (len = 0; (envp[ustk_envp_len])[len] != '\0'; len ++);

    ustk_pt_1 -= len + 1;
    ustk_envp[ustk_envp_len] = ustk_pt_1;

    strcpy(ustk_pt_1, envp[ustk_envp_len]);
  }

  int ustk_argv_len = 0;
  char * ustk_argv[10];
  if (argv != NULL)
  for (ustk_argv_len = 0; argv[ustk_argv_len] != NULL; ustk_argv_len ++) {
    printf("uload argv[%d] : %s\n", ustk_argv_len, argv[ustk_argv_len]);
    size_t len;

    for (len = 0; (argv[ustk_argv_len])[len] != '\0'; len ++);

    ustk_pt_1 -= len + 1;
    ustk_argv[ustk_argv_len] = ustk_pt_1;

    strcpy(ustk_pt_1, argv[ustk_argv_len]);
    printf("uload argv[%d] : %s\n", ustk_argv_len, ustk_pt_1);
  }

  uint32_t pt = (uint32_t)(ustk_pt_1 - 1); 
  pt = pt & ~0x3;

  uintptr_t * ustk_pt_4 = (uintptr_t *) pt;
  printf("pt : %p %p\n", ustk_pt_1, ustk_pt_4);

  ustk_pt_4 --; // prevent setting ustk_pt_1 to null
  * ustk_pt_4 = (uintptr_t)NULL;
  ustk_pt_4 --;

  for (int i = ustk_envp_len - 1; i >= 0; i --) { // cannot i = ustk_envp_len places the top NULL, since ustk_envp[len]'s value is not set
    printf("envp[%d], placing %p with %p\n", i, ustk_pt_4, ustk_envp[i]);
    * ustk_pt_4 = (uintptr_t)ustk_envp[i];
    ustk_pt_4 --;
  }

  * ustk_pt_4 = (uintptr_t)NULL;
  ustk_pt_4 --;

  for (int i = ustk_argv_len - 1; i >= 0; i --) { // cannot i = ustk_argv_len places the top NULL
    printf("argv[%d], placing %p with %p\n", i, ustk_pt_4, ustk_argv[i]);
    * ustk_pt_4 = (uintptr_t)ustk_argv[i];
    ustk_pt_4 --;
  }

  * ustk_pt_4 = (uintptr_t)ustk_argv_len;
  
  // loading exe
  uintptr_t entry = loader(pcb, filename);

  pcb->cp = ucontext(&pcb->as, kstack , (void*)entry);
  
  // pcb->cp->GPRx = (uintptr_t)ustk_pt_4; // GPRx = stack.top
  pcb->cp->GPRx = ustk_get_va((uint32_t)pcb->as.area.end, (uint32_t)ptr, (uint32_t)ustk_pt_4); 
  printf("uload placing sp at %p\n", ustk_pt_4);

  printf("uload finished %s\n", filename);
}