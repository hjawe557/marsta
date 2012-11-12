#include <stm32l.h>
#include <gpio.h>
#include <nvic.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <debug-uart.h>
#include <sys/process.h>
#include <sys/procinit.h>	
#include <etimer.h>
#include <sys/autostart.h>
#include <clock.h>
#include <usb-api.h>
#include <cdc-acm/cdc-acm.h>
#include <dev/serial-line.h>
#include <net/netstack.h>
#include "net/rime.h"
#include <rf230hal.h>
#include "random.h"
#include "stcn75.h"
#include "stcn75.c"
#include <leds.h>


/*---------------------------------------------------------------------------*/
PROCESS(temp_process, "temp");
AUTOSTART_PROCESSES(&temp_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(temp_process, ev, data)
{
	uint16_t tmp;
	char tempstr[32];
	static struct etimer et;
	PROCESS_BEGIN();
		
        printf("Initiating stcn75\r\n");
	stcn75_init();
	leds_on(LEDS_ALL);
	while(1) {
		etimer_set(&et, CLOCK_SECOND * 3);
                printf("waiting for the timer\r\n");
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		stcn75_read(&tmp);
		snprintf(tempstr, sizeof(tempstr), "%d", tmp);
    		printf("Measurement is done, tmp: %s\n\r",tempstr);
                leds_toggle(LEDS_ALL);
		
	}
	
  PROCESS_END();
}
