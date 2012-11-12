
#include <dev/leds.h>
#include <gpio.h>

void
leds_arch_init(void)
{
  RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN |
		   RCC_APB2ENR_IOPCEN);

  GPIO_CONF_OUTPUT_PORT(C, 6, PUSH_PULL, 10);

  /* LED off */
  GPIOC->BRR = 1<<6;
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
  /* printf("leds_arch_set %d\n", leds); */

  GPIOC->BSRR = 1<<6;

  if ((leds & LEDS_GREEN) || (leds == LEDS_ALL))
    GPIOC->BRR = 1<<6;

}
#if 0
void
leds_toggle(unsigned char leds)
{
}
void leds_off(unsigned char leds)
{
}

void leds_on(unsigned char leds)
{
}
#endif
