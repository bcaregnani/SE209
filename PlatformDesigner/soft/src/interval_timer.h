#ifndef INTERVAL_TIMER_H
#define INTERVAL_TIMER_H

void interval_timer_start();
unsigned int  interval_timer_val();
void interval_timer_init_periodic();
void interval_timer_ISR();
extern volatile int timer_tick;

#endif

