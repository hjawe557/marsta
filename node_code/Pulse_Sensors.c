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

extern volatile uint16_t  pulses;
/*---------------------------------------------------------------------------*/
PROCESS(Pulse_Sensors_process, "Pulse_Sensors");
AUTOSTART_PROCESSES(&Pulse_Sensors_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(Pulse_Sensors_process, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
    P2SEL&= 0x7f;
    P2IE |= 0x80;
    P2DIR &= 0x7f; // Set P2.3 to input direction
    P2IE |= 0x80; // P2.3 interrupt enabled
    P2IES |= 0x80; // P2.3 Hi/lo edge
    P2IFG &= ~0x80; // P2.3 IFG cleared
    _BIS_SR(GIE); //interrupt
    while (1){
                 static struct etimer et;                
                 etimer_set(&et, 9*CLOCK_SECOND);
                 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
                 etimer_set(&et, CLOCK_SECOND);		 
                 P2IE |= 0x80;
		 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	         P2IE |= 0x00;
		printf ("The wind speed:%d\n", pulses);
		pulses=0;
  }
exit:
  PROCESS_END();
}
