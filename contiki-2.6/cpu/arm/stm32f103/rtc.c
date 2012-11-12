
#include <stdint.h>
#include <stdio.h>
#include <nvic.h>
#include "rtc.h"
#include <debug-uart.h>

#define RTC_WAKEUP_IRQ_LINE 17

void rtc_sync(void)
{

        /* Datasheet 18.3.3 */

        /* Clear RSF flag */
        //RTC->CRL &= (uint16_t)~RTC_CRL_RSF;

        while ((RTC->CRL & RTC_CRL_RSF) == 0)
                ;
}

void rtc_pre(void)
{
        /* Datasheet 18.3.4 */

        /* Poll RTOFF, wait until its value goes to ‘1’ */
        while (0 == (RTC->CRL & RTC_CRL_RTOFF))
                ;

        /* Set the CNF bit to enter configuration mode */
        RTC->CRL |= RTC_CRL_CNF;
}

void rtc_post(void)
{
        /* Clear the CNF bit to exit configuration mode */
        RTC->CRL &= ~(RTC_CRL_CNF);

        /* Poll RTOFF, wait until its value goes to ‘1’ */
        while (0 == (RTC->CRL & RTC_CRL_RTOFF))
                ;
}

void rtc_set_alarm(uint32_t alarm)
{
	rtc_pre();
        /* MSB */
        RTC->ALRH = (alarm & (0xffff0000l)) >> 16;
        /* LSB */
        RTC->ALRL = (alarm & (0x0000ffffl));
	rtc_post();
}

static void rtc_setup(void)
{
  	/* Unmask interrupt */ 
  	EXTI->IMR |= (1 << RTC_WAKEUP_IRQ_LINE);
	/* Rising edge trigger */
  	EXTI->RTSR |= (1 << RTC_WAKEUP_IRQ_LINE);

#if 1
	NVIC_ENABLE_INT(RTCAlarm_IRQChannel);
	NVIC_SET_PRIORITY(RTCAlarm_IRQChannel, 2);
#else
	NVIC_ENABLE_INT(RTC_IRQChannel);
	NVIC_SET_PRIORITY(RTC_IRQChannel, 2);
#endif
	RTC->CNTL = 0;
	RTC->CNTH = 0;

	//Prescaler 1s
	RTC->PRLL = 0xfff;
        RTC->PRLH = 0x0000;

	// alarm interupt enable
	RTC->CRH |= RTC_CRH_ALRIE;
}

void rtc_init(void)
{
	int i;

  	RCC->APB1ENR |= (RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN);

	PWR->CR |= PWR_CR_DBP;
	
#define RTC_WAIT 10*1024*1024
#if 1
	RCC->CSR |= RCC_CSR_LSION;
	for (i=0; i<RTC_WAIT; i++) {
		if (RCC->CSR & RCC_CSR_LSIRDY) {
			RCC->BDCR |= (RCC_BDCR_RTCSEL_LSI | 
				RCC_BDCR_RTCEN);
			break;
		}
	}

	if (i == RTC_WAIT) {
		printf("Failed to enable RTC\r\n");
		return;
	}

#else
	RCC->BDCR |= (RCC_BDCR_LSEON);
	for (i=0; i<RTC_WAIT; i++) {
		if (RCC->BDCR & RCC_BDCR_LSERDY) {
			RCC->BDCR |= (RCC_BDCR_RTCSEL_LSE | 
				RCC_BDCR_RTCEN);
			break;
		}
	}

	if (i == RTC_WAIT) {
		printf("Failed to enable RTC\r\n");
		return;
	}
	
#endif

	printf("Pre\r\n");
	rtc_pre();
	printf("Init\r\n");
	rtc_setup();
	printf("Post\r\n");
	rtc_post();
	printf("Done\r\n");
	rtc_sync();
	printf("CRL: %x, CNTH: %x, CNTL: %x, ALRH: %x, ALRL: %x\r\n", 
		RTC->CRL, 
		RTC->CNTH, RTC->CNTL,
		RTC->ALRH, RTC->ALRL);
}

uint32_t rtc_read(void)
{
	rtc_sync();
	return RTC->CNTL;
}
static void
start_hse_clock(void)
{
  /* Start external oscillator */
  RCC->CR |= RCC_CR_HSEON;
  /* Wait for oscillator to stabilize */
  while(!(RCC->CR & RCC_CR_HSERDY))
    ;
}

static void
use_pll()
{
   RCC->CFGR = (RCC_CFGR_MCO_NOCLOCK
                | RCC_CFGR_PLLMULL6     /* PLL at 48MHz */
                | RCC_CFGR_PLLSRC       /* PLL runs on HSE */
                | RCC_CFGR_PPRE2_DIV1   /* APB2 at 48MHz */
                | RCC_CFGR_PPRE1_DIV2   /* APB1 at 24MHz */
                | RCC_CFGR_HPRE_DIV1    /* AHB at 48 MHz */
                | RCC_CFGR_USBPRE       /* USB clock at same speed as PLL */
                );
   RCC->CR |= RCC_CR_PLLON;
   /* Wait for PLL */
   while(!(RCC->CR & RCC_CR_PLLRDY));
   /* Switch to PLL as system clock */
   MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
}

void RTCAlarm_handler(void) __attribute__((interrupt));
void RTCAlarm_handler(void)
{
	uint32_t pending;

  	pending = EXTI->PR;
	if (pending & (1 << RTC_WAKEUP_IRQ_LINE)) {
    		EXTI->PR |= (1 << RTC_WAKEUP_IRQ_LINE);
	  	printf("RTC EXTI\r\n");

		RTC->CRL &= ~(RTC_CRL_ALRF);
		start_hse_clock();
		use_pll();
	}
}

void RTC_handler(void) __attribute__((interrupt));
void RTC_handler(void)
{
  	printf("RTC global interrupt\r\n");
	RTC->CRL &= ~(RTC_CRL_ALRF); //clear interupt flag.
}

