#include "stdint.h"
#include "nios2_irq.h"
#include "simple_printf.h"
#define GPIO_BASE 0x0000a000

typedef struct {
uint32_t data;
uint32_t enable;
uint32_t irq_mask;
uint32_t irq_pol;
uint32_t irq_ack;
} GPIOs;

// define macro to access to the different registers
#define PIO ((volatile GPIOs*) GPIO_BASE)


// prog gpio interrupt service routine
void prog_gpio_ISR()
{
  // Change polarity of the pin
  PIO->irq_pol = ~PIO->irq_pol;

  // Do something
  simple_printf("Interruption received \n");

  // Clear the interrupt
  PIO->irq_ack = 0;

}

int main()
{
  PIO->enable = 0xF;
  PIO->irq_mask = 0x8;
  PIO->irq_pol = 0x0;

  // Setting the interruptions
  RegisterISR(1, prog_gpio_ISR);
  irq_enable();

  while(1)
  {
    PIO->data = PIO->data;
  }

}