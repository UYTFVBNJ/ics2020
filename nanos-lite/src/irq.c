#include <common.h>

Context* do_syscall(Context *c);

Context* schedule(Context *prev);
static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD: 
      printf("event yield ID = %d\n", e.event); 
      c->epc += 4;
      break;
    case EVENT_SYSCALL:
      c = do_syscall(c);
      break;
    case EVENT_IRQ_TIMER:
      printf("event timer ID = %d\n", e.event); 
      c = schedule(c);
      c->epc += 4;
      break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
