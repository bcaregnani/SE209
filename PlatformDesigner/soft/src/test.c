// define macros to access to the data registers of
// the LEDs and SWs PIOs
#define LEDS  (*(volatile unsigned int*) 0x00009010)
#define SW    (*(volatile unsigned int*) 0x00009000)

int main()
{
  while(1)
    LEDS = SW;

}