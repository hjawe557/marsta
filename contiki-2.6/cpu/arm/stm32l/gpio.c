
#include <stdint.h>
#include <stm32l_gpio.h>
#include <gpio.h>


/*
Mode:
00: Input (reset state)
01: General purpose output mode
10: Alternate function mode
11: Analog mode
*/
void gpio_set_mode(GPIO_ *g, uint32_t pin, uint32_t mode) {
        /* Clear old mode */
        g->MODER &= ~(GPIO_MODE_MASK << (2 * pin));
        /* Set new mode */
        g->MODER |= mode << (2 * pin);
}

/*
Output type:
0: Output push-pull (reset state)
1: Output open-drain
*/
void gpio_set_output_type(GPIO_ *g, uint32_t pin, uint16_t type) {
        if (GPIO_OUTPUT_TYPE_PPULL == type) {
                g->OTYPER &= ~(1 << pin);
        } else {
                g->OTYPER |= (1 << pin);
        }
}

/*
Res:
00: No pull-up, pull-down
01: Pull-up
10: Pull-down
11: Reserved
*/
void gpio_set_resistors(GPIO_ *g, uint32_t pin, uint16_t res) {
        /* Clear old mode */
        g->PUPDR &= ~(GPIO_RESISTORS_MASK << (2 * pin));
        /* Set new mode */
        g->PUPDR |= (res << (2 * pin));
}
/*
Spd:
00: 400 kHz Very low speed
01: 2 MHz Low speed
10: 10 MHz Medium speed
11: 40 MHz High speed on 50 pF (50 MHz output max speed on 30 pF)
*/
void gpio_set_output_speed(GPIO_ *g, uint32_t pin, uint16_t spd) {
        /* Clear old mode */
        g->OSPEEDR &= ~(GPIO_OSPEED_MASK << (2 * pin));
        /* Set new mode */
        g->OSPEEDR |= (spd << (2 * pin));
}

void gpio_conf_output(GPIO_ *port, uint32_t pin, uint16_t mode, uint16_t spd) {
        gpio_set_mode(port, pin, GPIO_MODE_OUTPUT);
        gpio_set_output_type(port, pin, mode);
        gpio_set_output_speed(port, pin, spd);
}

void gpio_conf_input(GPIO_ *port, uint32_t pin, uint16_t res) {
        gpio_set_mode(port, pin, GPIO_MODE_INPUT);
        gpio_set_resistors(port, pin, res);
}

/*
  This is messy.

  af is a four-bit mask that defines a function for a specific pin. 

  Each GPIO port has two registers, AFRL and AFRH (AFR[0] and AFR[1]),
  where this mask can be set for each GPIO pin in the port. GPIO pins
  0-7 are mapped in AFRL/AFR[0] and pins 8-15 are mapped in
  AFRH/AFR[1].

  Ex:
  We want PB10 as USART3 TX.
  port: B, pin: 10 -> want to modify GPIOB->AFR[1].
  We use the af GPIO_AF_USART3 which is 0x7 (b0111)
  We shift 
  
*/

void gpio_map_af(GPIO_ *port, uint32_t pin, uint32_t af) {
        uint32_t afreg;
        uint32_t shift;

        /* AFRL or AFRH? */
        afreg = pin >> 0x03;
        /* Bits to shift in either AFRL or AFRH (& 0x07) */
        shift = (pin & 0x07) << 2;

        /* Clear previous mapping for pin */
        port->AFR[afreg] &= ~(((uint32_t)0xf) << shift);
        /* Set new mapping */
        port->AFR[afreg] |= (((uint32_t)(af)) << shift);
}

void gpio_conf_af(GPIO_ *port, uint32_t pin, uint16_t mode, 
                  uint16_t spd, uint16_t res) {
        gpio_set_mode(port, pin, GPIO_MODE_AF);
        gpio_set_output_speed(port, pin, spd);
        gpio_set_resistors(port, pin, res);
}

void gpio_output_set(GPIO_ *port, uint16_t pin) {
        /* Low part of BSRR is BS (bit set) and thus sets bit in ODR */
        port->BSRRL = (1 << pin);
}

void gpio_output_clear(GPIO_ *port, uint16_t pin) {
        /* High part of BSRR is BR (bit reset) and thus clears bit in ODR */
        port->BSRRH = (1 << pin);
}

int gpio_output_get(GPIO_ *port, uint16_t pin) {
        return port->ODR & (1 << pin);
}
