.section .text
.global setup_mair

setup_mair:
  mov x0, 0xFF
  msr MAIR_EL1,x0
  ret

.global setup_tcr

setup_tcr:
  movz x0 , #0x3510
  movk x0, #0x0080, lsl #16
  movk x0, #0x0002, lsl #32
  msr TCR_EL1, x0
  ret

.global setup_ttbr0

setup_ttbr0:
  msr TTBR0_EL1,x0
  ret

.global prepare_mmu

prepare_mmu:
  dsb ishst
  tlbi vmalle1
  dsb ish
  isb
  ret

.global enable_mmu

enable_mmu:
  mrs x0, SCTLR_EL1
  orr x0, x0 , #1 
  msr SCTLR_EL1, x0 
  isb
  ret

