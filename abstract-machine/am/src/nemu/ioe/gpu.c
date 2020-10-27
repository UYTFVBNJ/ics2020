#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

static AM_GPU_CONFIG_T AGC; // assume that screen size won't change


void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  int wh_info = inl(VGACTL_ADDR);
  int w = wh_info >> 16;  
  int h = wh_info & 0xffff;  

  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = w, .height = h,
    .vmemsz = w*h
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;

  // for (int i = 0; i < ctl->w; i++)
    // for (int j = 0; j < ctl->h; j++) 
      // if (ctl->x + i < AGC.width && ctl->y + j < AGC.height) 
        // fb[(ctl->x + i) * AGC.height/8 + (ctl->y + j)] = ((uint32_t *)ctl->pixels)[i * ctl->h + j];

  for (int i = 0; i < AGC.width; i++)
    for (int j = 0; j < AGC.height; j++) 
      // if (ctl->x + i < AGC.width && ctl->y + j < AGC.height) 
        fb[i * AGC.height + (ctl->y + j)] = 1;

  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}

void __am_gpu_init() {
  __am_gpu_config(&AGC);

  /*
  int i;
  int wh_info = inl(VGACTL_ADDR);
  int w = wh_info >> 16;  
  int h = wh_info & 0xffff;  
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 0);
  */
}