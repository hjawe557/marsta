#include "contiki.h"
#ifdef __IAR_SYSTEMS_ICC__
#include <msp430.h>
#else
#include <io.h>
#include <signal.h>
#endif
#include "dev/leds.h"
#include <stdlib.h>
#include <stdio.h>



/*---------------------------------------------------------------------------*/
PROCESS(isr_process, "isr");
AUTOSTART_PROCESSES(&isr_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(isr_process, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
    P2SEL&= 0x7f;
    P2IE |= 0x80;
    P2DIR &= 0x7f; // Set P2.7 to input direction
    P2IE |= 0x80; // P2.7 interrupt enabled
    P2IES |= 0x80; // P2.7 Hi/lo edge
    P2IFG &= ~0x80; // P2.7 IFG cleared
  while(1) {
    static struct etimer et;
    leds_on(LEDS_ALL);
    etimer_set(&et, CLOCK_SECOND);
    _BIS_SR(LPM4_bits + GIE); // Enter LPM4 w/interrupt
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //etimer_set(&et, CLOCK_SECOND);
    //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //leds_off(LEDS_ALL);
    //printf("Waiting \n");
  }

 exit:
  leds_off(LEDS_ALL);
  PROCESS_END();
}


