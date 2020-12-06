#include <am.h>
#include <riscv32.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  // printf("__am_irq_handle:");
  // printf("%d %d %d\n", c->epc, c->status, c->cause);
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
        if (c->epc >= 0x830000000) // might be changed
          ev.event = EVENT_SYSCALL; 
        else 
          ev.event = EVENT_YIELD; 
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
  return NULL;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
