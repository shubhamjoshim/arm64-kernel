.section .text

.global timer_get_frequency

timer_get_frequency:
  mrs x0, CNTFRQ_EL0 // this system registers stores the number of ticks the generic timer is doing per second
  ret

.global timer_set_ticks

timer_set_ticks:
  msr CNTP_TVAL_EL0, x0  // this system register stores the number of ticks till the timer expires
  ret

.global timer_enable

timer_enable:
// CNTP_CTL_EL0:
// bit 0 ENABLE  = 1 -> enable physical timer
// bit 1 IMASK   = 0 -> allow timer interrupt
// bit 2 ISTATUS     -> hardware timer status
  mov x0, #1
  msr CNTP_CTL_EL0, x0
  mov x0,#1
  msr CNTP_CTL_EL0,x0
  ret
