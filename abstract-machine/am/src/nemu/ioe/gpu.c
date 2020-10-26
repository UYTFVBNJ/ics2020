#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)


void __am_gpu_init() {
  int i;
  int wh_info = inl(VGACTL_ADDR);
  int w = wh_info >> 16;  
  int h = wh_info & 0xffff;  
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 0);
}

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
  
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
