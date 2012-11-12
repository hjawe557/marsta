#ifndef STM32_USART_H
#define STM32_USART_H

#include <stdint.h>

typedef struct {
        volatile uint16_t SR;
        const uint16_t R0;
        volatile uint16_t DR;
        const uint16_t R1;
        volatile uint16_t BRR;
        const uint16_t R2;
        volatile uint16_t CR1;
        const uint16_t R3;
        volatile uint16_t CR2;
        const uint16_t R4;
        volatile uint16_t CR3;
        const uint16_t R5;
        volatile uint16_t GTPR;
        const uint16_t R6;
} USART_;

#define USART1_BASE                     (APB2PERIPH_BASE + 0x3800)
#define USART2_BASE                     (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE                     (APB1PERIPH_BASE + 0x4800)

#define USART1                          ((USART_ *)USART1_BASE)
#define USART2                          ((USART_ *)USART2_BASE)
#define USART3                          ((USART_ *)USART3_BASE)

#define USART_SR_PE                     ((uint16_t)0x0001)
#define USART_SR_FE                     ((uint16_t)0x0002)
#define USART_SR_NE                     ((uint16_t)0x0004)
#define USART_SR_ORE                    ((uint16_t)0x0008)
#define USART_SR_IDLE                   ((uint16_t)0x0010)
#define USART_SR_RXNE                   ((uint16_t)0x0020)
#define USART_SR_TC                     ((uint16_t)0x0040)
#define USART_SR_TXE                    ((uint16_t)0x0080)
#define USART_SR_LBD                    ((uint16_t)0x0100)
#define USART_SR_CTS                    ((uint16_t)0x0200)

#define USART_CR1_SBK                   ((uint16_t)0x0001)
#define USART_CR1_RWU                   ((uint16_t)0x0002)
#define USART_CR1_RE                    ((uint16_t)0x0004)
#define USART_CR1_TE                    ((uint16_t)0x0008)
#define USART_CR1_IDLEIE                ((uint16_t)0x0010)
#define USART_CR1_RXNEIE                ((uint16_t)0x0020)
#define USART_CR1_TCIE                  ((uint16_t)0x0040)
#define USART_CR1_TXEIE                 ((uint16_t)0x0080)
#define USART_CR1_PEIE                  ((uint16_t)0x0100)
#define USART_CR1_PS                    ((uint16_t)0x0200)
#define USART_CR1_PCE                   ((uint16_t)0x0400)
#define USART_CR1_WAKE                  ((uint16_t)0x0800)
#define USART_CR1_M                     ((uint16_t)0x1000)
#define USART_CR1_UE                    ((uint16_t)0x2000)
#define USART_CR1_OVER8                 ((uint16_t)0x8000)

#define USART_CR2_ADD                   ((uint16_t)0x000f)
#define USART_CR2_LBDL                  ((uint16_t)0x0020)
#define USART_CR2_LBDIE                 ((uint16_t)0x0040)
#define USART_CR2_LBCL                  ((uint16_t)0x0100)
#define USART_CR2_CPHA                  ((uint16_t)0x0200)
#define USART_CR2_CPOL                  ((uint16_t)0x0400)
#define USART_CR2_CLKEN                 ((uint16_t)0x0800)

#define USART_CR2_STOP                  ((uint16_t)0x3000)
#define USART_CR2_STOP_0                ((uint16_t)0x1000)
#define USART_CR2_STOP_1                ((uint16_t)0x2000)

#define USART_CR2_LINEN                 ((uint16_t)0x4000)

#define USART_CR3_EIE                   ((uint16_t)0x0001)
#define USART_CR3_IREN                  ((uint16_t)0x0002)
#define USART_CR3_IRLP                  ((uint16_t)0x0004)
#define USART_CR3_HDSEL                 ((uint16_t)0x0008)
#define USART_CR3_NACK                  ((uint16_t)0x0010)
#define USART_CR3_SCEN                  ((uint16_t)0x0020)
#define USART_CR3_DMAR                  ((uint16_t)0x0040)
#define USART_CR3_DMAT                  ((uint16_t)0x0080)
#define USART_CR3_RTSE                  ((uint16_t)0x0100)
#define USART_CR3_CTSE                  ((uint16_t)0x0200)
#define USART_CR3_CTSIE                 ((uint16_t)0x0400)
#define USART_CR3_ONEBIT                ((uint16_t)0x0800)

#define USART_GTPR_PSC                  ((uint16_t)0x00ff)
#define USART_GTPR_PSC_0                ((uint16_t)0x0001)
#define USART_GTPR_PSC_1                ((uint16_t)0x0002)
#define USART_GTPR_PSC_2                ((uint16_t)0x0004)
#define USART_GTPR_PSC_3                ((uint16_t)0x0008)
#define USART_GTPR_PSC_4                ((uint16_t)0x0010)
#define USART_GTPR_PSC_5                ((uint16_t)0x0020)
#define USART_GTPR_PSC_6                ((uint16_t)0x0040)
#define USART_GTPR_PSC_7                ((uint16_t)0x0080)

#define USART_GTPR_GT                   ((uint16_t)0xff00)

#endif
