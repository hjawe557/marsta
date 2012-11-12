#include <lpc111x.h>
#include <sys/clock.h>
#include <sys/cc.h>
#include <sys/etimer.h>

static volatile clock_time_t current_clock = 0;
static volatile unsigned long current_seconds = 0;
static unsigned int second_countdown = CLOCK_SECOND;


void
SysTick_handler(void) __attribute__ ((interrupt));

void
SysTick_handler(void)
{
  /*
  (void)SysTick->CTRL;
  SCB->ICSR = SCB_ICSR_PENDSTCLR;
  */
  current_clock++;
  if(etimer_pending() && etimer_next_expiration_time() <= current_clock) {
    etimer_request_poll();
    /* printf("%d,%d\n\r", clock_time(),etimer_next_expiration_time  	()); */

  }
  if (--second_countdown == 0) {
    current_seconds++;
    second_countdown = CLOCK_SECOND;
  }
  
}


void
clock_init()
{
  // Set reload register
  SYSTICK_STRELOAD  = (CLOCK_SECOND & SYSTICK_STRELOAD_MASK) - 1;

  // Load the systick counter value
  SYSTICK_STCURR = 0;

  // Enable systick IRQ and timer
  SYSTICK_STCTRL = SYSTICK_STCTRL_CLKSOURCE |
                   SYSTICK_STCTRL_TICKINT |
                   SYSTICK_STCTRL_ENABLE;
}

clock_time_t
clock_time(void)
{
  return current_clock;
}

#if 1
/* The inner loop takes 4 cycles. The outer 5+SPIN_COUNT*4. */

#define SPIN_TIME 2 /* us */
#define SPIN_COUNT (((MCK*SPIN_TIME/1000000)-5)/4)

#ifndef __MAKING_DEPS__

void
clock_delay(unsigned int t)
{
#ifdef __THUMBEL__ 
  asm volatile("1: mov r1,%2\n2:\tsub r1,#1\n\tbne 2b\n\tsub %0,#1\n\tbne 1b\n":"=l"(t):"0"(t),"l"(SPIN_COUNT));
#else
#error Must be compiled in thumb mode
#endif
}
#endif
#endif /* __MAKING_DEPS__ */

unsigned long
clock_seconds(void)
{
  return current_seconds;
}
