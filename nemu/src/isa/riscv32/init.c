#include <isa.h>
#include <memory/paddr.h>

// this is not consistent with uint8_t
// but it is ok since we do not access the array directly
/*
static const uint32_t img [] = {
  0x800002b7,  // lui t0,0x80000
  0x0002a023,  // sw  zero,0(t0)
  0x0002a503,  // lw  a0,0(t0)
  0x0000006b,  // nemu_trap
};
*/

static const uint32_t img [] = {
  0x00000413,
  0x00009117,                                                                                                                                                                                               
  0xffc10113,                                                                                                                                                                                               
  0x024000ef,                                                                                                                                                                                               
                                                                                                                                                                                                         
  // main>:  ,                                                                                                                                                                                               
  0x00000513,                                                                                                                                                                                               
  0x00008067,                                                                                                                                                                                               
                                                                                                                                                                                                         
  // putch>: ,                                                                                                                                                                                               
  0xa10007b7,                                                                                                                                                                                               
  0x3ea78c23,                                                                                                                                                                                               
  0x00008067,                                                                                                                                                                                               
                                                                                                                                                                                                         
  // halt>:  ,                                                                                                                                                                                               
  0x00050513,                                                                                                                                                                                               
  0x0000006b,                                                                                                                                                                                               
  0x0000006f,                                                                                                                                                                                               
                                                                                                                                                                                                         
  // _trm_ini,                                                                                                                                                                                               
  0x80100537,                                                                                                                                                                                               
  0xff010113,                                                                                                                                                                                               
  0x05050513,                                                                                                                                                                                               
  0x00112623,                                                                                                                                                                                               
  0xfd1ff0ef,                                                                                                                                                                                               
  0x00050513,                                                                                                                                                                                               
  0x0000006b,                                                                                                                                                                                               
  0x0000006f,
};

static void restart() {
  /* Set the initial program counter. */
  cpu.pc = PMEM_BASE + IMAGE_START;

  /* The zero register is always 0. */
  cpu.gpr[0]._32 = 0;
}

void init_isa() {
  /* Load built-in image. */
  memcpy(guest_to_host(IMAGE_START), img, sizeof(img));

  /* Initialize this virtual computer system. */
  restart();
}
