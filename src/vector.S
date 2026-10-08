.section .vectors, "ax"
.balign 2048

.global exception_vectors

exception_vectors:
  b vector_sync_sp0
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
    b unexpected_exception

vector_irq_spx:
    b unexpected_exception

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

unexpected_exception:
    mrs x0, ELR_EL1
    mrs x1, ESR_EL1
    bl exception_handler
    b .
