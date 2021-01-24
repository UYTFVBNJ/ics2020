
#define concat_temp(x, y) x ## y
#define concat(x, y) concat_temp(x, y)
#define MAP(c, f) c(f)

#define REGS(f) \
      f( 1)       f( 3) f( 4) f( 5) f( 6) f( 7) f( 8) f( 9) \
f(10) f(11) f(12) f(13) f(14) f(15) f(16) f(17) f(18) f(19) \
f(20) f(21) f(22) f(23) f(24) f(25) f(26) f(27) f(28) f(29) \
f(30) f(31)

#define PUSH(n) sw concat(x, n), (n * 4)(sp);
#define POP(n)  lw concat(x, n), (n * 4)(sp);

#define CONTEXT_SIZE ((32 + 3 + 1) * 4)
#define OFFSET_SP     ( 2 * 4)
#define OFFSET_CAUSE  (32 * 4)
#define OFFSET_STATUS (33 * 4)
#define OFFSET_EPC    (34 * 4)
#define OFFSET_NP     (35 * 4)

.globl __am_asm_trap

__am_asm_trap:

  /* --- */

  csrrw sp, sscratch, sp   // (1) atomically exchange sp and sscratch

  bnez sp, save_context    // (2) take the branch if we trapped from user

  // if is in kernel sp = 0

  csrr sp, sscratch        // (3) if we trapped from kernel, restore the original sp

  

  /* --- */

save_context:



  addi sp, sp, -CONTEXT_SIZE



  MAP(REGS, PUSH)



  csrr t0, sscratch

  addi t0, t0, CONTEXT_SIZE

  sw t0, OFFSET_SP(sp)



  csrr t0, scause

  csrr t1, sstatus

  csrr t2, sepc



  sw t0, OFFSET_CAUSE(sp)

  sw t1, OFFSET_STATUS(sp)

  sw t2, OFFSET_EPC(sp)



  csrr t0, sscratch

  sltiu t1, t0, 1 // ksa == 0 => t1 == 1(kernel) / ksa != 0 => t1 == 0(user)

  sw t1, OFFSET_NP(sp)



  mv a0, sp

  jal __am_irq_handle



  mv sp, a0



  lw t1, OFFSET_STATUS(sp)

  lw t2, OFFSET_EPC(sp)

  csrw sstatus, t1

  csrw sepc, t2



  

  MAP(REGS, POP)



  /* --- */



  lw t5, OFFSET_NP(sp)

  // if t0 = c->np = 0

  bnez t5, set_sp

  // ksp = $sp

  csrw sscratch, sp

set_sp:

  lw sp, OFFSET_SP(sp)

  /* --- */

  sret

