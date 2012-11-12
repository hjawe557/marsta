#include <stdint.h>
/* #include <gpio.h> */

extern int main(void);

typedef void (*ISR_func)(void);
extern void* stack_entry;

  
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
  &stack_entry,
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
extern unsigned char _etext;
extern unsigned char _data;
extern unsigned char _edata;
extern unsigned char _bss;
extern unsigned char _ebss;
/*
extern uint8_t _data[];
extern uint8_t _etext[];
extern uint8_t _edata[];
*/
static void
copy_initialized(void)
{
  register unsigned char *src, *dst;

  src = &_etext;
  dst = &_data;
  while(dst < &_edata) {
    *dst++ = *src++;
  }
  /*
  uint8_t *ram = _data;
  uint8_t *rom = _etext;
  while(ram < _edata) {
    *ram++ = *rom++;
  }
  */
}

extern uint8_t __bss_start[];
extern uint8_t __bss_end[];

static void
clear_bss(void)
{
  register unsigned char *src, *dst;

  dst = &_bss;
  while(dst < &_ebss) {
    *dst++ = 0;
  }

  /*
  uint8_t *m = __bss_start;
  while(m < __bss_end) {
    *m++ = 0;
  }
  */

}

extern unsigned char _etext;
extern unsigned char _data;
extern unsigned char _edata;
extern unsigned char _bss;
extern unsigned char _ebss;


static void enable_fault_exceptions(void)
{
}


inline void sysinit(void)
{
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
