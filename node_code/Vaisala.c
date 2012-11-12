#include "contiki.h"
#ifdef __IAR_SYSTEMS_ICC__
#include <msp430.h>
#else
#include <io.h>
#include <signal.h>
#endif
#include <stdlib.h>
#include <stdio.h> /* For printf() */
#include <dev/serial-line.h>
#include <stdint.h>
#include <string.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <etimer.h>
#include <sys/autostart.h>
#include <clock.h>
#include "net/rime.h"
#include <net/netstack.h>
#include <leds.h>
#include "dev/uart0.h"
#include "dev/uart1.h"



 //extern process_event_t serial_line_event_message;
/*---------------------------------------------------------------------------*/
/* We declare the two processes */
PROCESS(Vaisala_process, "Vaisala process");

/* We require the processe to be started automatically */
AUTOSTART_PROCESSES(&Vaisala_process);
/*---------------------------------------------------------------------------*/
/* Implementation of the first process */
PROCESS_THREAD(Vaisala_process, ev, data)
{
// variables are declared static to ensure their values are kept
// between kernel calls.
// any process must start with this.
PROCESS_BEGIN();

//leds_on(LEDS_ALL);
printf("I started \n");
uart0_init(BAUD2UBR(19200));
while (1)
{
// wait here for an event to happen
PROCESS_WAIT_EVENT();
if(ev == serial_line_event_message && data != NULL)
 {
leds_toggle(LEDS_ALL);
// do the process work
printf("I recieved :%s\n",data);
printf("%d",strlen(data));

}
// and loop
/*while(1) {
    static struct etimer et;
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    leds_on(LEDS_ALL);
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    leds_off(LEDS_ALL);
  }*/

  PROCESS_END();
}
}
// any process must end with this, even if it is never reached.
//PROCESS_END();
//}
/*---------------------------------------------------------------------------*/
