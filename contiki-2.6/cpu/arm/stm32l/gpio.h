#ifndef GPIO_H
#define GPIO_H

#include <stm32l.h>
#include <stm32l_gpio.h>

void gpio_set_mode(GPIO_ *g, uint32_t pin, uint32_t mode);
void gpio_set_output_type(GPIO_ *g, uint32_t pin, uint16_t type);
void gpio_set_resistors(GPIO_ *g, uint32_t pin, uint16_t res);
void gpio_set_output_speed(GPIO_ *g, uint32_t pin, uint16_t spd);
void gpio_conf_output(GPIO_ *port, uint32_t pin, uint16_t mode, uint16_t spd);
void gpio_conf_af(GPIO_ *port, uint32_t pin, uint16_t mode, uint16_t spd, uint16_t res);
void gpio_map_af(GPIO_ *port, uint32_t pin, uint32_t af);
void gpio_output_set(GPIO_ *port, uint16_t pin);
void gpio_output_clear(GPIO_ *port, uint16_t pin);
int gpio_output_get(GPIO_ *port, uint16_t pin);
void gpio_conf_input(GPIO_ *port, uint32_t pin, uint16_t res);

#endif
