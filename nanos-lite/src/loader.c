#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t ramdisk_read(void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
  printf("loading:\n");
  static Elf_Ehdr eh;
  static Elf_Phdr ph;

  ramdisk_read(&eh, 0, 52);

  //
  printf("eh: %d\n", eh.e_phoff);
  printf("eh: %d\n", eh.e_phentsize);
  printf("eh: %d\n", eh.e_phnum);

  for (int i = 0; i < eh.e_phnum; i ++) {
    ramdisk_read(&ph, eh.e_phoff + i * eh.e_phentsize, eh.e_phentsize);

    //
    printf("ph: %d\n", ph.p_type);
    printf("ph: %p\n", ph.p_offset);
    printf("ph: %p\n", ph.p_vaddr);
    printf("ph: %d\n", ph.p_filesz);
    printf("ph: %d\n", ph.p_memsz);
    

  }




  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

