#ifndef STM32L_H
#define STM32L_H

#include <stdint.h>

#define IO8(a)                          (*(volatile uint8_t *)(a))
#define IO16(a)                         (*(volatile uint16_t *)(a))
#define IO32(a)                         (*(volatile uint32_t *)(a))

#define FLASH_BASE                      ((uint32_t)0x08000000)
#define SRAM_BASE                       ((uint32_t)0x20000000)

#define PERIPHERAL_BASE                 ((uint32_t)0x40000000)
#define APB1PERIPH_BASE                 (PERIPHERAL_BASE)
#define APB2PERIPH_BASE                 (PERIPHERAL_BASE + 0x10000)
#define AHBPERIPH_BASE                  (PERIPHERAL_BASE + 0x20000)

#define SET_BIT(REG, BIT)               ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)             ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)              ((REG) & (BIT))
#define CLEAR_REG(REG)                  ((REG) = 0x0)
#define WRITE_REG(REG, VAL)             ((REG) = VAL)
#define READ_REG(REG)                   ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~CLEARMASK)) | (SETMASK)))

#endif
