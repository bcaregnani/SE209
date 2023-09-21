#include "interval_timer.h"
#include "address_map_nios2.h"

volatile int timer_tick = 0;

// All registers are 16 bits but 4 bytes aligned
typedef struct {
   int status;
   int control;
   int c_start_l;
   int c_start_h;
   int c_capture_l;
   int c_capture_h;
} IT_t;

#define timer ((volatile IT_t *) TIMER_BASE)

void interval_timer_start()
{
   // init to max value
   // start once
   // if the timer reaches 0 it will stop
   timer->c_start_l = 0xFFFF;
   timer->c_start_h = 0xFFFF;
   // control register
   // bit 0: ITO   -> if 1 generates irq
   // bit 1: CONT  -> if 0 count once
   // bit 2: START -> writting 1 starts the timer
   // bit 3: STOP  -> writting 1 stops the timer
   timer->control = 0x4;
}

unsigned int interval_timer_val(){
  // write to the capture register to sample the counter register
  timer->c_capture_h = 0;
  unsigned int cl = timer->c_capture_l & 0xFFFF;
  unsigned int ch = timer->c_capture_h & 0xFFFF;
  return 0xFFFFFFFFU - ((ch<<16)|cl) ;
}

void interval_timer_init_periodic()
{
   // set interval TODO: could be an argument
   // 1/(100 MHz) x (50000000) = 500 msec
   int counter = 50000000;
   timer->c_start_l = (counter) & 0xFFFF;
   timer->c_start_h = (counter>>16) & 0xFFFF;
   // control register
   // bit 0: ITO   -> if 1 generates irq
   // bit 1: CONT  -> if 0 count once
   // bit 2: START -> writting 1 starts the timer
   // bit 3: STOP  -> writting 1 stops the timer
   timer->control = 0x7;
}

void interval_timer_ISR( )
{
   // clear the interrupt
   timer->status = 0;

   // TODO: do something
   timer_tick = 1;
}
