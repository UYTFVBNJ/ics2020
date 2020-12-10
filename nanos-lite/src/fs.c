#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, DEV_EVT, CTL_FB, DEV_FB, PROC_DIF,};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
  [DEV_EVT]   = {"/dev/events", 0, 0, events_read, invalid_write},
  [CTL_FB]    = {"???fbctl", 0, 0, invalid_read, invalid_write},
  [DEV_FB]    = {"/dev/fb", 0, 0, invalid_read, invalid_write},
  [PROC_DIF]  = {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},
#include "files.h"
};

#define FT_SIZE (sizeof(file_table)/sizeof(Finfo))

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

extern AM_GPU_CONFIG_T dispinfo; // might be changed

size_t open_offset[FT_SIZE];

void init_fs() {
  Log("Initializing file system");

  // TODO: initialize the size of /dev/fb
  
  file_table[DEV_FB].size = dispinfo.vmemsz;


  for (int i = 0; i < FT_SIZE; i ++) {
    if (file_table[i].read == NULL)  file_table[i].read  = ramdisk_read;
    if (file_table[i].write == NULL) file_table[i].write = ramdisk_write;
  }
}

int fs_open(const char *pathname, int flags, int mode) {
  for (int i = 0; i < FT_SIZE; i ++) 
    if (strcmp(pathname, file_table[i].name) == 0) {
      return i;
    }
  assert(0);
  return 0;
}

size_t fs_read(int fd, void *buf, size_t len) {
  if (open_offset[fd] + len >= file_table[fd].size) 
    len = file_table[fd].size - 1 - open_offset[fd];

  size_t l = file_table[fd].read(buf, file_table[fd].disk_offset + open_offset[fd], len);
  
  open_offset[fd] += l;
  return l;
}

size_t fs_write(int fd, const void *buf, size_t len) {
  if (open_offset[fd] + len >= file_table[fd].size) 
    len = file_table[fd].size - 1 - open_offset[fd];

  size_t l = file_table[fd].write(buf, file_table[fd].disk_offset + open_offset[fd], len);
  
  open_offset[fd] += l;
  return l;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  size_t target_offset;
  switch (whence) {
    case SEEK_SET:
      target_offset = offset;
      break;
    case SEEK_CUR:
      target_offset = open_offset[fd] + offset;
      break;
    case SEEK_END:
      target_offset = file_table[fd].size + offset;
      break;
    default:
      assert(0);
      break;
  }
  if (target_offset > file_table[fd].size) 
    target_offset = file_table[fd].size;

  open_offset[fd] = target_offset; 
  return open_offset[fd];
}

int fs_close(int fd) {
  return 0;
}