#include "interval_timer.h"
#include "simple_printf.h"
#include "stdint.h"
#include "nios2_irq.h"
#include <string.h>
#define DMA_BASE 0x0000a000
#define NUM 80



typedef struct {
    uint32_t k[4];
    uint32_t src;
    uint32_t dest;
    uint32_t num;
    uint32_t ctrl;
} DMA_t;


// define macro to access to the different registers
#define DMA ((volatile DMA_t*) DMA_BASE)

volatile uint8_t irq_received;
volatile uint32_t time_elapsed;

void avalon_accel_ISR()
{
    // Reset interruption
    DMA->ctrl = 0;
    // Measure time
    time_elapsed = interval_timer_val(); // * (1/50000); // *(1/50000000) * 1000 (ms)
    // Interruption received
    irq_received = 1;
    
}


uint32_t src[NUM*2];
uint32_t dst[NUM*2];


int main()
{   
    uint32_t value = 0xbade0000;
    irq_received = 0;
    time_elapsed = 0;

    // Set the DMA registers
    DMA->k[0] = 0xdeadbeef;
    DMA->k[1] = 0xc01dcafe;
    DMA->k[2] = 0xbadec0de;
    DMA->k[3] = 0x8badf00d;
    DMA->src = (uint32_t) &src[0];
    DMA->dest = (uint32_t) &dst[0];
    DMA->num = (uint32_t) NUM;



    // Setting values in memory zone
    for (uint16_t i = 0; i < NUM*2; i++)
    {
        src[i] = value++;
    };

    
    // Start the timer
    interval_timer_start();

    memcpy(&dst, &src, sizeof(uint32_t)*NUM*2);

    time_elapsed = interval_timer_val();

    simple_printf("Time elapsed for copying a memory block %d ticks \n", time_elapsed);
    

    // Setting the interruptions
    RegisterISR(0, avalon_accel_ISR);
    irq_enable();

    // Start the timer
    interval_timer_start();

    // Start the DMA
    DMA->ctrl = 0x00000001;

    // frequency used 50Mhz
    // 1/(50Mhz) * ticks = time elapsed

    while (1)
    {

        if (irq_received)
        {   
            irq_received = 0;
            simple_printf("Time elapsed for cyphering the memory block %d ticks \n", time_elapsed);
        }
        
    }
    
    

}