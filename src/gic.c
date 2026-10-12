#include <stdint.h>

static volatile uint32_t *const gic_master = (volatile uint32_t *)0x08000000; //master toggle that enables the interrupt side of gic
static volatile uint32_t *const cpu_master = (volatile uint32_t *)0x08010000; //master toggle for cpu
static volatile uint32_t *const gic_priority = (volatile uint32_t *)0x08010004; //address of priority filter
static volatile uint32_t *const gic_enabler0 = (volatile uint32_t *)0x08000100;


void gic_enable_timer_irq(void){
  // GICD_ISENABLER0 controls interrupt IDs 0-31.
  // Each bit corresponds to one interrupt ID.
  // We want to enable interrupt ID 30 (the EL1 physical timer),
  // so we write a value with only bit 30 set.
  // IMPORTANT:
  // ISENABLER is a "write-1-to-set" register:
  //   write 1 -> enable that interrupt
  //   write 0 -> leave that interrupt unchanged
  // So this does NOT disable any interrupts that were already enabled.
  *gic_master = 1U;
  *cpu_master = 1U;
  *gic_enabler0 = (1U << 30);
  *gic_priority = 0xFFU; //allow all interrupts through, setting threshold to the highest value(lowest prority)
}
