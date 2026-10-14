.global _start

_start:
  msr SPSel, #1
  ldr x0, =stack_top
  mov sp, x0
  bl set_bss
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

.global set_bss

  set_bss:
  ldr x0,=bss_start
  ldr x1,=bss_end
  b bss_clear
.global bss_clear
  bss_clear:
  cmp x0, x1
  b.hs bss_done //can't do ret here cuz ret is a instruction do we have to call a label to do that

  strb wzr, [x0], #1
  b bss_clear


.global bss_done
  bss_done:
  ret
