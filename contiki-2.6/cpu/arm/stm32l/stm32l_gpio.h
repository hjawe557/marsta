#ifndef STM32L_GPIO_H
#define STM32L_GPIO_H

#include "stm32l.h"

typedef struct {
	volatile uint32_t MODER;
	volatile uint16_t OTYPER;
	const uint16_t R0;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint16_t IDR;
	const uint16_t R1;
	volatile uint16_t ODR;
	const uint16_t R2;
	volatile uint16_t BSRRL;
	volatile uint16_t BSRRH;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];
} GPIO_;

#define GPIOA_BASE                      (AHBPERIPH_BASE + 0x0000)
#define GPIOB_BASE                      (AHBPERIPH_BASE + 0x0400)
#define GPIOC_BASE                      (AHBPERIPH_BASE + 0x0800)
#define GPIOD_BASE                      (AHBPERIPH_BASE + 0x0C00)
#define GPIOE_BASE                      (AHBPERIPH_BASE + 0x1000)
#define GPIOH_BASE                      (AHBPERIPH_BASE + 0x1400)

#define GPIOA                           ((GPIO_ *)GPIOA_BASE)
#define GPIOB                           ((GPIO_ *)GPIOB_BASE)
#define GPIOC                           ((GPIO_ *)GPIOC_BASE)
#define GPIOD                           ((GPIO_ *)GPIOD_BASE)
#define GPIOE                           ((GPIO_ *)GPIOE_BASE)
#define GPIOGH                          ((GPIO_ *)GPIOH_BASE)

#define GPIO_MODE_MASK                  3
#define GPIO_MODE_INPUT                 0
#define GPIO_MODE_OUTPUT                1
#define GPIO_MODE_AF                    2
#define GPIO_MODE_ANALOG                3

#define GPIO_OUTPUT_TYPE_PPULL          0
#define GPIO_OUTPUT_TYPE_ODRAIN         1

#define GPIO_RESISTORS_MASK             3
#define GPIO_RESISTORS_NONE             0
#define GPIO_RESISTORS_PULLUP           1
#define GPIO_RESISTORS_PULLDN           2
#define GPIO_RESISTORS_RESERVED         3

#define GPIO_OSPEED_MASK                3
#define GPIO_OSPEED_400KHZ              0
#define GPIO_OSPEED_2MHZ                1
#define GPIO_OSPEED_10MHZ               2
#define GPIO_OSPEED_40MHZ               3

#define GPIO_AF_RTC_50Hz                ((uint8_t)0x0)
#define GPIO_AF_MCO                     ((uint8_t)0x0)
#define GPIO_AF_RTC_AF1                 ((uint8_t)0x0)
#define GPIO_AF_WKUP                    ((uint8_t)0x0)
#define GPIO_AF_SWJ                     ((uint8_t)0x0)
#define GPIO_AF_TRACE                   ((uint8_t)0x0)
#define GPIO_AF_TIM2                    ((uint8_t)0x1)
#define GPIO_AF_TIM3                    ((uint8_t)0x2)
#define GPIO_AF_TIM4                    ((uint8_t)0x2)
#define GPIO_AF_TIM9                    ((uint8_t)0x3)
#define GPIO_AF_TIM10                   ((uint8_t)0x3)
#define GPIO_AF_TIM11                   ((uint8_t)0x3)
#define GPIO_AF_I2C1                    ((uint8_t)0x4)
#define GPIO_AF_I2C2                    ((uint8_t)0x4)
#define GPIO_AF_SPI1                    ((uint8_t)0x5)
#define GPIO_AF_SPI2                    ((uint8_t)0x5)
#define GPIO_AF_USART1                  ((uint8_t)0x7)
#define GPIO_AF_USART2                  ((uint8_t)0x7)
#define GPIO_AF_USART3                  ((uint8_t)0x7)
#define GPIO_AF_USB                     ((uint8_t)0xa)
#define GPIO_AF_LCD                     ((uint8_t)0xb)
#define GPIO_AF_RI                      ((uint8_t)0xe)
#define GPIO_AF_EVENTOUT                ((uint8_t)0xf)

#endif
