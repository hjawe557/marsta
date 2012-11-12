
#include <dev/leds.h>
#include <gpio.h>

void
leds_arch_init(void)
{
  RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN |
		   RCC_APB2ENR_IOPFEN);

  GPIO_CONF_OUTPUT_PORT(F, 6, PUSH_PULL, 10);
  GPIO_CONF_OUTPUT_PORT(F, 7, PUSH_PULL, 10);
  GPIO_CONF_OUTPUT_PORT(F, 8, PUSH_PULL, 10);
  GPIO_CONF_OUTPUT_PORT(F, 9, PUSH_PULL, 10);

  /* LED off */
  GPIOF->BRR = 1<<6;
  GPIOF->BRR = 1<<7;
  GPIOF->BRR = 1<<8;
  GPIOF->BRR = 1<<9;
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

  GPIOF->BRR = 1<<6;
  GPIOF->BRR = 1<<7;
  GPIOF->BRR = 1<<8;
  GPIOF->BRR = 1<<9;

  if ((leds & LEDS_GREEN) || (leds == LEDS_ALL))
    GPIOF->BSRR = 1<<6;

  if ((leds & LEDS_YELLOW) || (leds == LEDS_ALL))
    GPIOF->BSRR = 1<<7;

  if ((leds & LEDS_RED) || (leds == LEDS_ALL))
    GPIOF->BSRR = 1<<8;

  if ((leds & LEDS_BLUE) || (leds == LEDS_ALL))
    GPIOF->BSRR = 1<<9;
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
