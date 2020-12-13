#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  size_t i;
  for (i = 0; i < len; i ++) putch(((char*)buf)[i]);
  return i;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode != AM_KEY_NONE) {
    return snprintf(buf, len + 1,"k%c %s\n", (ev.keydown) ? ('d') : ('u'), keyname[ev.keycode]);
  }
  return 0;
}

AM_GPU_CONFIG_T dispinfo; // might be changed
size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  printf("%d\n", len);
  dispinfo = io_read(AM_GPU_CONFIG);
  printf("WIDTH : %d \nHEIGHT : %d\n", dispinfo.width, dispinfo.height);
  printf("%d\n", len);
  snprintf(buf, len + 1,"WIDTH : %d \nHEIGHT : %d\n", dispinfo.width, dispinfo.height);
  printf("%d\n", len);
  printf("%s\n", buf);
  return snprintf(buf, len + 1,"WIDTH : %d \nHEIGHT : %d\n", dispinfo.width, dispinfo.height);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
	io_write(AM_GPU_FBDRAW, offset % dispinfo.width, offset / dispinfo.width, buf, len, 1, true);
  return 0;
}

static inline int getnum(char * buf) {
  while (!(*buf >= '0' && *buf <= '9') && (*buf != '-') && (*buf != '+')) buf++;
  int number;
  if (sscanf(buf, "%d", &number) == 1) {
    return number;
  }
  return -1;
}

void init_device() {
  Log("Initializing devices...");
  dispinfo = io_read(AM_GPU_CONFIG);
  ioe_init();
}
