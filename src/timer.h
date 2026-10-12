#ifndef TIME_H
#define TIME_H
#include <stdint.h>

uint64_t timer_get_frequency(void);
void timer_set_ticks(uint64_t ticks);
void timer_enable(void);

#endif
