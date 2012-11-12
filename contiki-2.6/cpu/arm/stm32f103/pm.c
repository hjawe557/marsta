#include <stdint.h>
#include "pm.h"

void pm_stop_mode(int regulator_off)
{
        uint32_t tmp;

        /* Select the regulator state in STOP mode */
        tmp = PWR->CR;
        
        /* Clear PDDS and LPDS bits */
        tmp &= ~(PWR_CR_PDDS | PWR_CR_LPDS);
        
        if (regulator_off) {
                /* Set LPDS bit according to PWR_Regulator value */
                tmp |= PWR_CR_LPDS;
        }

        /* Store the new value */
        PWR->CR = tmp;

        /* Set SLEEPDEEP bit of Cortex System Control Register */
        SCB->SCR |= SCB_SCR_SLEEPDEEP;

        asm("wfi"::);
        /* __WFI(); */
        /* __WFE(); */
}

