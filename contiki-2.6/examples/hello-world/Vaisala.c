#include "contiki.h"
#include <stdio.h> /* For printf() */
#include <dev/serial-line.h>
#include <stm32l.h>
#include <stdint.h>
#include <string.h>
#include <debug-uart.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <etimer.h>
#include <sys/autostart.h>
#include <clock.h>
#include "net/rime.h"
#include <net/netstack.h>
#include <leds.h>




extern process_event_t serial_line_event_message;
/*---------------------------------------------------------------------------*/
/* We declare the two processes */
PROCESS(hello_world_process, "Hello world process");

/* We require the processe to be started automatically */
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
/* Implementation of the first process */
PROCESS_THREAD(hello_world_process, ev, data)
{
// variables are declared static to ensure their values are kept
// between kernel calls.
// any process must start with this.
PROCESS_BEGIN();

leds_on(LEDS_ALL);
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
}
// any process must end with this, even if it is never reached.
PROCESS_END();
}
/*---------------------------------------------------------------------------*/
