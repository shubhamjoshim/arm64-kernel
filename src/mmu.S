.section .text
.global setup_mair

setup_mair:
  mov x0, 0xFF
  msr MAIR_EL1,x0
  ret

.global read_mair

read_mair:
  mrs x0, MAIR_EL1
  ret
