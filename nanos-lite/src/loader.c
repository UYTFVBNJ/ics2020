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

static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename, 0, 0);
  printf("loading\n");
  static Elf_Ehdr eh;
  static Elf_Phdr ph;

  fs_lseek(fd, 0, 0);
  fs_read(fd, &eh, 52);

  //
  // printf("eh: %d\n", eh.e_phoff);
  // printf("eh: %d\n", eh.e_phentsize);
  // printf("eh: %d\n", eh.e_phnum);

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
      fs_read(fd, (void(*))ph.p_vaddr, ph.p_filesz);

      for (size_t j = ph.p_vaddr + ph.p_filesz; j < ph.p_vaddr + ph.p_memsz; j ++) {
        *((uint32_t *)j) = 0;
      }
    }
  }

  return eh.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

