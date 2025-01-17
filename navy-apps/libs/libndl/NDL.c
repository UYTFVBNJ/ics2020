#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static struct timeval tv;

uint32_t NDL_GetTicks() {
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int NDL_PollEvent(char *buf, int len) {
  int fd = 0;
  if (fd == 0) fd = open("/dev/events", 0, 0);

  if (read(fd, buf, len) > 0) return 1;

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

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    printf("HI~ NDL_OpenCanvas_NWM_APP:\n");
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  } else {
    char buf[64];
    read(6, buf, sizeof(buf) - 1); //snprintf to be fixed
    printf("HI~ NDL_OpenCanvas_default:\n %s\n", buf); 
    screen_w = getnum(buf);
    screen_h = getnum(buf + 13);  
    if (*w == 0 && *h == 0) {
      *w = screen_w; *h = screen_h;
    }
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  if (w == 0 && h == 0) {
    w = screen_w; h = screen_h;
  }
  for (int i = 0; i < h; i ++) {
    lseek(5, (y + i) * screen_w + x, SEEK_SET);
    write(5, pixels + i * w, w);
  }
}

  void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  return 0;
}

void NDL_Quit() {
}
