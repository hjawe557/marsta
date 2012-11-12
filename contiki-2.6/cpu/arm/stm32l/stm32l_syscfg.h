#ifndef STM32L_SYSCFG_H
#define STM32L_SYSCFG_H

#include <stm32l.h>

typedef struct
{
        volatile uint32_t MEMRMP;
        volatile uint32_t PMC;
        volatile uint32_t EXTICR[4];
} SYSCFG_;

#define SYSCFG_BASE                     (APB2PERIPH_BASE + 0x0000)
#define SYSCFG                          ((SYSCFG_ *)SYSCFG_BASE)


#define SYSCFG_PMC_USB_PU               ((uint32_t)0x00000001)

#endif
