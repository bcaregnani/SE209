#include "pushbutton.h"
#include "address_map_nios2.h"

volatile int pb_count  = 0;

// all registers are only 4bits width
typedef struct {
   int data;
   int unused;
   int mask;
   int captured;
} PB_t;

#define pb ((volatile PB_t *) KEY_BASE)

void pushbutton_init()
{
   // Enable interrupts for all pushbuttons
   pb->mask  = 0xF;
}

void pushbutton_ISR( void )
{
   int press;
   // Read the pushbutton interrupt register
   press = pb->captured ;

   // TODO: do something
   if(press & 0x1) pb_count = 0;
   else {
      if(press & 0x2) pb_count = pb_count + 1;
      if(press & 0x4) pb_count = pb_count - 1;
   }

   // Clear the interrupt
   pb->captured  = press;
}
