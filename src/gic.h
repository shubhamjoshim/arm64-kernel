#ifndef GIC_H
#define GIC_H

#include<stdint.h>
void gic_enable_timer_irq(void);
uint32_t gic_acknowledge_irq(void);
uint32_t gic_get_irq_id(uint32_t iar);
void gic_end_irq(uint32_t iar);

#endif
