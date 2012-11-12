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

extern volatile uint16_t  precipitation;
/*---------------------------------------------------------------------------*/
PROCESS(Pulse_Sensors_process, "Pulse_Sensors");
AUTOSTART_PROCESSES(&Pulse_Sensors_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(Pulse_Sensors_process, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
    P2SEL&= 0xf7;
    P2IE |= 0x08;
    P2DIR &= 0xf7; // Set P2.3 to input direction
    P2IE |= 0x08; // P2.3 interrupt enabled
    P2IES |= 0x08; // P2.3 Hi/lo edge
    P2IFG &= ~0x08; // P2.3 IFG cleared
    _BIS_SR(GIE); //interrupt
    while (1){
		 P2IE |= 0x08; 
		 static struct etimer et;
		 etimer_set(&et, CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
			 P2IE |= 0x00;
		printf ("precipitation in main function is:%d\n", precipitation);
		precipitation=0;
     etimer_set(&et, 9*CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }
exit:
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
