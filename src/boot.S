.global _start

_start:
  msr SPSel, #1
  ldr x0, =stack_top
  mov sp, x0
  ldr x0,=exception_vectors
  msr VBAR_EL1,x0
  isb //make sure the cpu recognises the register before continuing 
  bl kernel_main
  b .


.global get_current_el  
  get_current_el: 
  mrs x0, CurrentEL
  lsr x0,x0, #2
  ret
