#include "nios2_irq.h"
#include "interval_timer.h"
#include "pushbutton.h"
#include "simple_printf.h"


int main(void)
{

   interval_timer_start();
   simple_printf("Let's start...\n");
   unsigned int d = interval_timer_val();
   simple_printf("Last print took %d cycles\n",d);

   RegisterISR(0, interval_timer_ISR);
   RegisterISR(1, pushbutton_ISR);

   interval_timer_init_periodic();
   pushbutton_init();

   irq_enable();

   int t = 0;

   while(1)
   {
      if (timer_tick)
      {
         t ++;
         timer_tick = 0;
         simple_printf("*%d*: %04d\r",t/2, pb_count);

      }
   }

   // never return
   for(;;)
      ;
   return 0;
}
