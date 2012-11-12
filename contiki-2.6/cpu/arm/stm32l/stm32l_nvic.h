#ifndef STM32L_NVIC_H
#define STM32L_NVIC_H

#include <stm32l_scb.h>

typedef struct {
	volatile uint32_t ISER[2];
        const uint32_t R0[30];
	volatile uint32_t ICER[2];
	const uint32_t R1[30];
	volatile uint32_t ISPR[2];
	const uint32_t R2[30];
	volatile uint32_t ICPR[2];
	const uint32_t R3[30];
	volatile uint32_t IABR[2];
	const uint32_t R4[62];
	volatile uint32_t IPR[15];
} NVIC_;

#define NVIC_BASE                       (SCS_BASE + 0x0100)
#define NVIC                            ((NVIC_ *)NVIC_BASE)

#endif
