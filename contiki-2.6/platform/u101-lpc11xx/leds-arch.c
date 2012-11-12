
#include <dev/leds.h>

/*
  On u101v2 we have two LEDs. LED1 on PD2 and LED2 on PC6.
*/

void
leds_arch_init(void)
{

}

unsigned char
leds_arch_get(void)
{

  unsigned char on = 0;
  /*
  if (*AT91C_PIOA_ODSR & GREEN_PIN) on |= LEDS_GREEN;
  if (*AT91C_PIOA_ODSR & YELLOW_PIN) on |= LEDS_YELLOW;
  if (*AT91C_PIOA_ODSR & RED_PIN) on |= LEDS_RED;
  */
  return on;
}

void
leds_arch_set(unsigned char leds)
{
  /* printf("leds_arch_set %d\n\r", leds); */

  if ((leds & LEDS_GREEN) || (leds == LEDS_ALL))
    ;


  if ((leds & LEDS_RED) || (leds == LEDS_ALL))
    ;

}
