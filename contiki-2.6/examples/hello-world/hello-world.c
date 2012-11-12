#include "contiki.h"
#include <stdio.h> /* For printf() */
#include <dev/serial-line.h>
//#include <stm32l.h>
#include <stdint.h>
#include <string.h>
//#include <debug-uart.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <etimer.h>
#include <sys/autostart.h>
#include <clock.h>
#include "net/rime.h"
#include <net/netstack.h>
#include <leds.h>

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
static struct etimer timer;
static int count = 0;

// any process must start with this.
PROCESS_BEGIN();

// set the etimer module to generate an event in one second.
etimer_set(&timer, CLOCK_CONF_SECOND);
leds_on(LEDS_ALL);
while (1)
{
// wait here for an event to happen
PROCESS_WAIT_EVENT();
// if the event is the timer event as expected...
if(ev == PROCESS_EVENT_TIMER)
 {
leds_toggle(LEDS_ALL);
// do the process work
printf("Hello, world  #%i\n", count);
count ++;
// reset the timer so it will generate an other event
// the exact same time after it expired (periodicity guaranteed)
etimer_reset(&timer);
}
// and loop
}
// any process must end with this, even if it is never reached.
PROCESS_END();
}
/*---------------------------------------------------------------------------*/
