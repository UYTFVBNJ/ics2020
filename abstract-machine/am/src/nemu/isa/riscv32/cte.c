#include <am.h>
#include <riscv32.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

void __am_get_cur_as(Context *c);
void __am_switch(Context *c);
Context* __am_irq_handle(Context *c) {
  __am_get_cur_as(c);
  // printf("__am_irq_handle:");
  // printf("%p %d %d\n", c->epc, c->status, c->cause);
  // printf("%p %p\n", c, c->gpr[2]);
  // for (int i = 0; i < 32; i ++) printf("%d ", c->gpr[i]); 
  // printf("\n");
  // for (int i = 0; i < 32; i ++) printf("%d ", &c->gpr[i]); 
  // printf("\n");
  // printf("%d ", &c->pdir); 
  // printf("\n");

  if (user_handler) {
    Event ev = {0};
    switch (c->cause) {
      case 1: // Supervisor software interrupt
        // printf("addr: %p\n", c->epc);
        // if (c->epc >= 0x83000000) // might be changed
          ev.event = EVENT_SYSCALL; 
        // else 
          // ev.event = EVENT_YIELD; 
        break; 
      case 5: // Supervisor timer interrupt
        ev.event = EVENT_ERROR; 
        break; 
      case 9: // Supervisor external interrupt
        ev.event = EVENT_ERROR; 
        break; 
      default: ev.event = EVENT_ERROR;  break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  printf("%p %d %d\n", c->epc, c->status, c->cause);
  printf("switch: %p %p\n", c, c->pdir);
  
  __am_switch(c);
  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw stvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  // printf("kcontext:\n");
  uint32_t * base = (uint32_t *)kstack.end - (32 + 3);
  // printf("base:%p\n", base);

  for (int i = 0; i < 32; i ++) *(base + i) = 0; // pdir = NULL

  *(base + 10) = (uint32_t)arg; // arg0

  *(base + 2) = (uint32_t)kstack.end; // sp

  *(base + 32) = 0; // CAUSE
  *(base + 33) = 0; // STATUS
  *(base + 34) = (uint32_t)entry; // EPC

  return (Context*)base;
}

void yield() {
  // asm volatile("li a7, -1; ecall");
  asm volatile("li a7, 1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
