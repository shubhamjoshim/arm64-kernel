.section .vectors, "ax"
.balign 2048

.global exception_vectors

.macro SAVE_CONTEXT
    sub sp,sp,#272
    stp x0,x1, [sp,#0]
    stp x2,x3, [sp,#16]
    stp x4,x5, [sp,#32]
    stp x6,x7, [sp,#48]
    stp x8,x9, [sp,#64]
    stp x10,x11, [sp,#80]
    stp x12,x13, [sp,#96]
    stp x14,x15, [sp,#112]
    stp x16,x17, [sp,#128]
    stp x18,x19, [sp,#144]
    stp x20,x21, [sp,#160]
    stp x22,x23, [sp,#176]
    stp x24,x25, [sp,#192]
    stp x26,x27, [sp,#216]
    stp x28,x30, [sp,#232]

    mrs x0, ELR_EL1 //same for system registers too
    str x0,[sp,#248]

    mrs x1, SPSR_EL1
    str x1,[sp, #256] 

    mrs x2, ESR_EL1
    str x2,[sp, #264]

.endm

.macro RESTORE_CONTEXT

    ldr x0, [sp,#248] //loading the special registers first
    msr ELR_EL1 , x0
    ldr x0,[sp,#256]
    msr SPSR_EL1,x0
    ldp x0,x1,[sp,#0]
    ldp x0,x1,[sp,#16]
    ldp x0,x1,[sp,#32]
    ldp x0,x1,[sp,#48]
    ldp x0,x1,[sp,#64]
    ldp x0,x1,[sp,#80]
    ldp x0,x1,[sp,#96]
    ldp x0,x1,[sp,#112]
    ldp x0,x1,[sp,#128]
    ldp x0,x1,[sp,#144]
    ldp x0,x1,[sp,#160]
    ldp x0,x1,[sp,#176]
    ldp x0,x1,[sp,#192]
    ldp x0,x1,[sp,#208]
    ldp x0,x1,[sp,#224]

    add sp,sp,#272 //free up the stack
.endm
exception_vectors: //declaring exception vectors according to arm64 rules 
  b vector_sync_sp0 //each entry in the vector table is given 128 bytes
  .space 124

  b vector_irq_sp0
  .space 124

  b vector_fiq_sp0
  .space 124

  b vector_serror_sp0
  .space 124

  b vector_sync_spx
  .space 124

  b vector_irq_spx
  .space 124

  b vector_fiq_spx
  .space 124

  b vector_serror_spx
  .space 124
  
  b vector_sync_lower_aarch64
  .space 124

  b vector_irq_lower_aarch64
  .space 124

  b vector_fiq_lower_aarch64
  .space 124

  b vector_serror_lower_aarch64
  .space 124

  b vector_sync_lower_aarch32
  .space 124

  b vector_irq_lower_aarch32
  .space 124

  b vector_fiq_lower_aarch32
  .space 124

  b vector_serror_lower_aarch32
  .space 124

vector_sync_sp0:
    b unexpected_exception

vector_irq_sp0:
    b unexpected_exception

vector_fiq_sp0:
    b unexpected_exception

vector_serror_sp0:
    b unexpected_exception

vector_sync_spx:
    b sync_exception;

vector_irq_spx:
    b irq_exception

vector_fiq_spx:
    b unexpected_exception

vector_serror_spx:
    b unexpected_exception

vector_sync_lower_aarch64:
    b unexpected_exception

vector_irq_lower_aarch64:
    b unexpected_exception

vector_fiq_lower_aarch64:
    b unexpected_exception

vector_serror_lower_aarch64:
    b unexpected_exception

vector_sync_lower_aarch32:
    b unexpected_exception

vector_irq_lower_aarch32:
    b unexpected_exception

vector_fiq_lower_aarch32:
    b unexpected_exception

vector_serror_lower_aarch32:
    b unexpected_exception

sync_exception: 
    SAVE_CONTEXT
    mov x0,sp //move the value of sp to x0 cuz c expects it's arguments starting from x0
    bl exception_handler
    RESTORE_CONTEXT
    eret //exception handling is done, control is given back to boot.S/kerner.c
irq_exception:
  SAVE_CONTEXT
  mov x0, sp
  bl irq_handler
  RESTORE_CONTEXT
  eret
unexpected_exception:
  b .

