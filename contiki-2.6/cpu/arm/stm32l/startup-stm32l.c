#include <stdint.h>
#include "stm32l_rcc.h"
#include "stm32l_scb.h"
#include "stm32l_flash.h"
#include "stm32l_pwr.h"
#include <gpio.h>

extern int main(void);

typedef void (*ISR_func)(void);

  
#define SECTION(x) __attribute__ ((section(#x)))
#define ISR_VECTOR_SECTION SECTION(.isr_vector)
void sys_reset(void) __attribute__((naked));
void NMI_handler(void) __attribute__((interrupt));
void HardFault_handler(void) __attribute__((interrupt));
void MemManage_handler(void) __attribute__((interrupt));
void BusFault_handler(void) __attribute__((interrupt));
void UsageFault_handler(void) __attribute__((interrupt));

static void unhandled_int(void) __attribute__((interrupt));

#define UNHANDLED_ALIAS __attribute__((weak, alias("unhandled_int")));
void Main_Stack_End(void);
void HardFault_handler(void)__attribute__((weak, alias("dHardFault_handler")));
void MemManage_handler(void)__attribute__((weak, alias("dMemManage_handler")));
void BusFault_handler(void) __attribute__((weak, alias("dBusFault_handler")));
void UsageFault_handler(void)__attribute__((weak, alias("dUsageFault_handler")));
void Reserved_handler(void) UNHANDLED_ALIAS;
void SVC_handler(void) UNHANDLED_ALIAS;
void DebugMon_handler(void) UNHANDLED_ALIAS;
void PendSV_handler(void) UNHANDLED_ALIAS;
void SysTick_handler(void) UNHANDLED_ALIAS;
void WWDG_IRQhandler(void) UNHANDLED_ALIAS;
void PVD_IRQhandler(void) UNHANDLED_ALIAS;
void TAMPER_STAMP_IRQhandler(void) UNHANDLED_ALIAS;
void RTC_WKUP_IRQhandler(void) UNHANDLED_ALIAS;
void FLASH_IRQhandler(void) UNHANDLED_ALIAS;
void RCC_IRQhandler(void) UNHANDLED_ALIAS;
void EXTI0_IRQhandler(void) UNHANDLED_ALIAS;
void EXTI1_IRQhandler(void) UNHANDLED_ALIAS;
void EXTI2_IRQhandler(void) UNHANDLED_ALIAS;
void EXTI3_IRQhandler(void) UNHANDLED_ALIAS;
void EXTI4_IRQhandler(void) UNHANDLED_ALIAS;
void DMA1_Channel1_IRQhandler(void) UNHANDLED_ALIAS;
void DMA1_Channel2_IRQhandler(void) UNHANDLED_ALIAS;
void DMA1_Channel3_IRQhandler(void) UNHANDLED_ALIAS;
void DMA1_Channel4_IRQhandler(void) UNHANDLED_ALIAS;
void DMA1_Channel5_IRQhandler(void) UNHANDLED_ALIAS;
void DMA1_Channel6_IRQhandler(void) UNHANDLED_ALIAS;
void DMA1_Channel7_IRQhandler(void) UNHANDLED_ALIAS;
void ADC1_IRQhandler(void) UNHANDLED_ALIAS;
void USB_HP_IRQhandler(void) UNHANDLED_ALIAS;
void USB_LP_IRQhandler(void) UNHANDLED_ALIAS;
void COMP_IRQhandler(void) UNHANDLED_ALIAS;
void DAC_IRQhandler(void) UNHANDLED_ALIAS;
void CAN_RX1_handler(void) UNHANDLED_ALIAS;
void CAN_SCE_handler(void) UNHANDLED_ALIAS;
void EXTI9_5_IRQhandler(void) UNHANDLED_ALIAS;
void LCD_IRQhandler(void) UNHANDLED_ALIAS;
void TIM1_BRK_handler(void) UNHANDLED_ALIAS;
void TIM1_UP_handler(void) UNHANDLED_ALIAS;
void TIM1_TRG_COM_handler(void) UNHANDLED_ALIAS;
void TIM1_CC_handler(void) UNHANDLED_ALIAS;
void TIM2_IRQhandler(void) UNHANDLED_ALIAS;
void TIM3_IRQhandler(void) UNHANDLED_ALIAS;
void TIM4_IRQhandler(void) UNHANDLED_ALIAS;
void I2C1_EV_IRQhandler(void) UNHANDLED_ALIAS;
void I2C1_ER_IRQhandler(void) UNHANDLED_ALIAS;
void I2C2_EV_IRQhandler(void) UNHANDLED_ALIAS;
void I2C2_ER_IRQhandler(void) UNHANDLED_ALIAS;
void SPI1_IRQhandler(void) UNHANDLED_ALIAS;
void SPI2_IRQhandler(void) UNHANDLED_ALIAS;
void USART1_IRQhandler(void) UNHANDLED_ALIAS;
void USART2_IRQhandler(void) UNHANDLED_ALIAS;
void USART3_IRQhandler(void) UNHANDLED_ALIAS;
void EXTI15_10_IRQhandler(void) UNHANDLED_ALIAS;
void RTC_Alarm_IRQhandler(void) UNHANDLED_ALIAS;
void USBWakeup_handler(void) UNHANDLED_ALIAS; 
void TIM8_BRK_handler(void) UNHANDLED_ALIAS;
void TIM8_UP_handler(void) UNHANDLED_ALIAS;
void TIM8_TRG_COM_handler(void) UNHANDLED_ALIAS;
void TIM8_CC_handler(void) UNHANDLED_ALIAS;
void TIM9_IRQhandler(void) UNHANDLED_ALIAS;
void TIM10_IRQhandler(void) UNHANDLED_ALIAS;
void TIM11_IRQhandler(void) UNHANDLED_ALIAS;
void ADC3_handler(void) UNHANDLED_ALIAS;
void FSMC_handler(void) UNHANDLED_ALIAS;
void SDIO_handler(void) UNHANDLED_ALIAS;
void TIM5_handler(void) UNHANDLED_ALIAS;
void SPI3_handler(void) UNHANDLED_ALIAS;
void UART4_handler(void) UNHANDLED_ALIAS;
void UART5_handler(void) UNHANDLED_ALIAS;
void TIM6_IRQhandler(void) UNHANDLED_ALIAS;
void TIM7_IRQhandler(void) UNHANDLED_ALIAS;
void USB_FS_WKUP_IRQhandler(void) UNHANDLED_ALIAS;

const ISR_func isr_vector[61] ISR_VECTOR_SECTION = {
  Main_Stack_End,  /* This is set in the linker script */
  sys_reset,
  NMI_handler,
  HardFault_handler,
  MemManage_handler,
  BusFault_handler,
  UsageFault_handler,
  0,
  0,
  0,
  0,
  SVC_handler,
  DebugMon_handler,
  0,
  PendSV_handler,
  SysTick_handler,
  /* STM32 */
  WWDG_IRQhandler,
  PVD_IRQhandler,
  TAMPER_STAMP_IRQhandler,
  RTC_WKUP_IRQhandler,
  FLASH_IRQhandler,
  RCC_IRQhandler,
  EXTI0_IRQhandler,
  EXTI1_IRQhandler,
  EXTI2_IRQhandler,
  EXTI3_IRQhandler,
  EXTI4_IRQhandler,
  DMA1_Channel1_IRQhandler,
  DMA1_Channel2_IRQhandler,
  DMA1_Channel3_IRQhandler,
  DMA1_Channel4_IRQhandler,
  DMA1_Channel5_IRQhandler,
  DMA1_Channel6_IRQhandler,
  DMA1_Channel7_IRQhandler,
  ADC1_IRQhandler,
  USB_HP_IRQhandler,
  USB_LP_IRQhandler,
  DAC_IRQhandler,
  COMP_IRQhandler,
  EXTI9_5_IRQhandler,
  LCD_IRQhandler,
  TIM9_IRQhandler,
  TIM10_IRQhandler,
  TIM11_IRQhandler,
  TIM2_IRQhandler,
  TIM3_IRQhandler,
  TIM4_IRQhandler,
  I2C1_EV_IRQhandler,
  I2C1_ER_IRQhandler,
  I2C2_EV_IRQhandler,
  I2C2_ER_IRQhandler,
  SPI1_IRQhandler,
  SPI2_IRQhandler,
  USART1_IRQhandler,
  USART2_IRQhandler,
  USART3_IRQhandler,
  EXTI15_10_IRQhandler,
  RTC_Alarm_IRQhandler,
  USB_FS_WKUP_IRQhandler,
  TIM6_IRQhandler,
  TIM7_IRQhandler,
  /*


   Reserved_handler,
   sys_reset,
   NMI_handler,
   HardFault_handler,
   MemManage_handler,
   BusFault_handler,
   UsageFault_handler,
   0,
   0,
   0,
   0,
   SVCall_handler,
   DebugMonitor_handler,
   0,
   PendSV_handler,
   SysTick_handler,
   WWDG_IRQhandler,
   PVD_IRQhandler,
   TAMPER_STAMP_IRQhandler,
   RTC_WKUP_IRQhandler,
   FLASH_IRQhandler,
   RCC_IRQhandler,
   EXTI0_IRQhandler,
   EXTI1_IRQhandler,
   EXTI2_IRQhandler,
   EXTI3_IRQhandler,
   EXTI4_IRQhandler,
   DMA1_Channel1_IRQhandler,
   DMA1_Channel2_IRQhandler,
   DMA1_Channel3_IRQhandler,
   DMA1_Channel4_IRQhandler,
   DMA1_Channel5_IRQhandler,
   DMA1_Channel6_IRQhandler,
   DMA1_Channel7_IRQhandler,
   ADC1_IRQhandler,
   USB_HP_IRQhandler,
   USB_LP_IRQhandler,
   DAC_IRQhandler,
   COMP_IRQhandler,
   EXTI9_5_IRQhandler,
   LCD_IRQhandler,
   TIM9_IRQhandler,
   TIM10_IRQhandler,
   TIM11_IRQhandler,
   TIM2_IRQhandler,
   TIM3_IRQhandler,
   TIM4_IRQhandler,
   I2C1_EV_IRQhandler,
   I2C1_ER_IRQhandler,
   I2C2_EV_IRQhandler,
   I2C2_ER_IRQhandler,
   SPI1_IRQhandler,
   SPI2_IRQhandler,
   USART1_IRQhandler,
   USART2_IRQhandler,
   USART3_IRQhandler,
   EXTI15_10_IRQhandler,
   RTC_Alarm_IRQhandler,
   USB_FS_WKUP_IRQhandler,
   TIM6_IRQhandler,
   TIM7_IRQhandler,
   0,
   0,
   0,
   0,
   0,
   0 
  */
/* BootRAM */
};

extern uint8_t _data[];
extern uint8_t _etext[];
extern uint8_t _edata[];

static void
copy_initialized(void)
{
  uint8_t *ram = _data;
  uint8_t *rom = _etext;
  while(ram < _edata) {
    *ram++ = *rom++;
  }
}

extern uint8_t __bss_start[];
extern uint8_t __bss_end[];

static void
clear_bss(void)
{
  uint8_t *m = __bss_start;
  while(m < __bss_end) {
    *m++ = 0;
  }
}



static void enable_fault_exceptions(void)
{
  SCB->SHCSR |= (SCB_SHCSR_MEMFAULTENA | SCB_SHCSR_BUSFAULTENA
		 | SCB_SHCSR_USGFAULTENA);
}

static void flash_setup(void) {
        /* Enable 64-bit access */
        FLASH->ACR |= FLASH_ACR_ACC64;
                
        /* Enable Prefetch Buffer */
        FLASH->ACR |= FLASH_ACR_PRFTEN;
        
        /* Flash 1 wait state */
        FLASH->ACR |= FLASH_ACR_LATENCY;
}

inline int hse_setup(void)
{
        const uint32_t retries = 1024;
        uint32_t i;

        /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
        /* Enable HSE */
        RCC->CR |= ((uint32_t)RCC_CR_HSEON);
        
        /* Wait till HSE is ready and if Time out is reached exit */
        for (i=0; i<retries; i++) {
                if (RCC->CR & RCC_CR_HSERDY) {
                        break;
                }
        } 
        
        if (i == retries) {
                /* HSE not ready */
                return -1;
        }

        flash_setup();

        /* Power enable */
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
                
        /* Select the Voltage Range 1 (1.8 V) */
        PWR->CR = PWR_CR_VOS_0;
                
        /* Wait Until the Voltage Regulator is ready */
        while((PWR->CSR & PWR_CSR_VOSF) != 0)
                ;
                
        /* HCLK = SYSCLK /1 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
        
        /* PCLK2 = HCLK /1*/
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
        
        /* PCLK1 = HCLK /1*/
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
        
        /*  PLL configuration */
#if USB_NEEDS_48_MHZ
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL |
                                            RCC_CFGR_PLLDIV));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL8 | RCC_CFGR_PLLDIV2);
#else
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL |
                                            RCC_CFGR_PLLDIV));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLDIV3);
#endif
        /* Enable PLL */
        RCC->CR |= RCC_CR_PLLON;
        
        /* Wait till PLL is ready */
        while((RCC->CR & RCC_CR_PLLRDY) == 0)
                ;
        
        /* Select PLL as system clock source */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;
        
        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
                ;

        return 0;
}

inline void sysinit(void)
{
        /* Set MSION bit */
        RCC->CR |= RCC_CR_MSION;

        /* Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], MCOSEL[2:0] and MCOPRE[2:0] bits */
        RCC->CFGR &= (uint32_t)0x88FFC00C;
        
        /* Reset HSION, HSEON, CSSON and PLLON bits */
        RCC->CR &= (uint32_t)0xEEFEFFFE;
        
        /* Reset HSEBYP bit */
        RCC->CR &= (uint32_t)0xFFFBFFFF;
        
        /* Reset PLLSRC, PLLMUL[3:0] and PLLDIV[1:0] bits */
        RCC->CFGR &= (uint32_t)0xFF02FFFF;
        
        /* Disable all interrupts */
        RCC->CIR = 0x00000000;
        
        /* Configure the System clock frequency, AHB/APBx prescalers and Flash settings */
        hse_setup();
        
#ifdef VECT_TAB_SRAM
        SCB->VTOR = SRAM_BASE; /* Vector Table Relocation in Internal SRAM. */
#else
        SCB->VTOR = FLASH_BASE; /* Vector Table Relocation in Internal FLASH. */
#endif
}


void sys_reset(void)
{
  copy_initialized();
  clear_bss();
  enable_fault_exceptions();

  sysinit();


  main();
  while(1);
  
}

void
NMI_handler(void)
{
  while(1);
}


static void
unhandled_int(void)
{
  while(1);
}

static void
dHardFault_handler(void)
{
  while(1);
}

static void
dUsageFault_handler(void)
{
  while(1);
}

static void
dMemManage_handler(void)
{
  while(1);
}

static void
dBusFault_handler(void)
{
  while(1);
}
