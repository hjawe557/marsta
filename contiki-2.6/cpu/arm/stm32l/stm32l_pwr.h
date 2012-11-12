#ifndef STM32L_PWR_H
#define STM32L_PWR_H



typedef struct
{
        volatile uint32_t CR;
        volatile uint32_t CSR;
} PWR_;


#define PWR_BASE                        (APB1PERIPH_BASE + 0x7000)
#define PWR                             ((PWR_ *) PWR_BASE)

#define PWR_CR_LPSDSR                   ((uint16_t)0x0001)
#define PWR_CR_PDDS                     ((uint16_t)0x0002)
#define PWR_CR_CWUF                     ((uint16_t)0x0004)
#define PWR_CR_CSBF                     ((uint16_t)0x0008)
#define PWR_CR_PVDE                     ((uint16_t)0x0010)

#define PWR_CR_PLS                      ((uint16_t)0x00E0)
#define PWR_CR_PLS_0                    ((uint16_t)0x0020)
#define PWR_CR_PLS_1                    ((uint16_t)0x0040)
#define PWR_CR_PLS_2                    ((uint16_t)0x0080)

#define PWR_CR_PLS_LEV0                 ((uint16_t)0x0000)
#define PWR_CR_PLS_LEV1                 ((uint16_t)0x0020)
#define PWR_CR_PLS_LEV2                 ((uint16_t)0x0040)
#define PWR_CR_PLS_LEV3                 ((uint16_t)0x0060)
#define PWR_CR_PLS_LEV4                 ((uint16_t)0x0080)
#define PWR_CR_PLS_LEV5                 ((uint16_t)0x00A0)
#define PWR_CR_PLS_LEV6                 ((uint16_t)0x00C0)
#define PWR_CR_PLS_LEV7                 ((uint16_t)0x00E0)

#define PWR_CR_DBP                      ((uint16_t)0x0100)
#define PWR_CR_ULP                      ((uint16_t)0x0200)
#define PWR_CR_FWU                      ((uint16_t)0x0400)

#define PWR_CR_VOS                      ((uint16_t)0x1800)
#define PWR_CR_VOS_0                    ((uint16_t)0x0800)
#define PWR_CR_VOS_1                    ((uint16_t)0x1000)
#define PWR_CR_LPRUN                    ((uint16_t)0x4000)

#define PWR_CSR_WUF                     ((uint16_t)0x0001)
#define PWR_CSR_SBF                     ((uint16_t)0x0002)
#define PWR_CSR_PVDO                    ((uint16_t)0x0004)
#define PWR_CSR_VREFINTRDYF             ((uint16_t)0x0008)
#define PWR_CSR_VOSF                    ((uint16_t)0x0010)
#define PWR_CSR_REGLPF                  ((uint16_t)0x0020)

#define PWR_CSR_EWUP1                   ((uint16_t)0x0100)
#define PWR_CSR_EWUP2                   ((uint16_t)0x0200)
#define PWR_CSR_EWUP3                   ((uint16_t)0x0400)


#endif
