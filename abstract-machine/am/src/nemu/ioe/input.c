#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  int key_info = inl(KBD_ADDR);
  kbd->keydown = key_info & KEYDOWN_MASK; 
  kbd->keycode = key_info ? key_info ^ KEYDOWN_MASK : 0;
}
