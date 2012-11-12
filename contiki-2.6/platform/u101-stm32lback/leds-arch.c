
#include <stm32l_rcc.h>
#include <dev/leds.h>
#include <stm32l_gpio.h>
#include <gpio.h>

/*
  On u101v2 we have two green LEDs. LED1 on PD2 and LED2 on PC6.
  Since red is very similar to green, we pretend that the one on PC6 is red...
*/

void
leds_arch_init(void)
{
  RCC->AHBENR |= (RCC_AHBENR_GPIOCEN |
                  RCC_AHBENR_GPIODEN);

  gpio_conf_output(GPIOC, 6, GPIO_OUTPUT_TYPE_PPULL, GPIO_OSPEED_400KHZ);
  gpio_conf_output(GPIOD, 2, GPIO_OUTPUT_TYPE_PPULL, GPIO_OSPEED_400KHZ);

  gpio_output_set(GPIOC, 6);
  gpio_output_set(GPIOD, 2);
}

unsigned char
leds_arch_get(void)
{

  unsigned char on = 0;

  return on;
}

void
leds_arch_set(unsigned char leds)
{
  /* printf("leds_arch_set %02x\n", (uint32_t)leds); */

  if ((leds & LEDS_RED) || (leds == LEDS_ALL))
    gpio_output_clear(GPIOC, 6);
  else
   gpio_output_set(GPIOC, 6);

  if ((leds & LEDS_GREEN) || (leds == LEDS_ALL))
    gpio_output_clear(GPIOD, 2);
  else
    gpio_output_set(GPIOD, 2);
  
}
