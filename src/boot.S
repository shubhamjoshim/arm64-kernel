.global _start

_start:
  ldr x0, =stack_top
  mov sp, x0
  bl kernel_main
  b .


.global get_current_el  
  get_current_el: 
  mrs x0, CurrentEL
  lsr x0,x0, #2
  ret
