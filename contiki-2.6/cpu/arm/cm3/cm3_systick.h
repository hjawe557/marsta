#ifndef CM3_SYSTICK_H
#define CM3_SYSTICK_H

#include <stdint.h>
#include <cm3_scb.h>

typedef struct {
	volatile uint32_t CTRL;
	volatile uint32_t LOAD;
	volatile uint32_t VAL;
        const volatile uint32_t CALIB;
} SysTick_;

#define SysTick_BASE                    (SCS_BASE + 0x0010)
#define SysTick                         ((SysTick_ *)SysTick_BASE)

#define SysTick_CTRL_ENABLE             ((uint32_t)0x00000001)
#define SysTick_CTRL_TICKINT            ((uint32_t)0x00000002)
#define SysTick_CTRL_CLKSOURCE          ((uint32_t)0x00000004)
#define SysTick_CTRL_COUNTFLAG          ((uint32_t)0x00010000)

#define SysTick_LOAD_RELOAD             ((uint32_t)0x00ffffff)

#define SysTick_VAL_CURRENT             ((uint32_t)0x00ffffff)

#define SysTick_CALIB_TENMS             ((uint32_t)0x00ffffff)
#define SysTick_CALIB_SKEW              ((uint32_t)0x40000000)
#define SysTick_CALIB_NOREF             ((uint32_t)0x80000000)

#endif
