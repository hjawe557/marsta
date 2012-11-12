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
#include <dev/leds.h>
#include <dev/serial-line.h>
#include "net/mac/sicslowmac.h"
#include "net/rime.h"
#include <net/netstack.h>
#include <rf230hal.h>
#include "net/mac/frame802154.h"
#include "contiki.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "stcn75.h"
#include "stcn75.c"
/*---------------------------------------------------------------------------*/
PROCESS(nodetest_process, "nodetest");
AUTOSTART_PROCESSES(&nodetest_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	//int temp;
	
  	printf("broadcast message received from %d.%d: '%s'\r\n",
        	 from->u8[0], from->u8[1], (char *)packetbuf_dataptr());

}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(nodetest_process, ev, data)
{
	uint16_t tmp;
	char tempstr[32];
	static struct etimer et;
	PROCESS_BEGIN();
	
	broadcast_open(&broadcast, 129, &broadcast_call);
	printf("Initiating stcn75\r\n");
	stcn75_init();
	
	while(1) {
		etimer_set(&et, CLOCK_SECOND * 3);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		//Send temp
		stcn75_read(&tmp);
		snprintf(tempstr, sizeof(tempstr), "%d", tmp);

		packetbuf_copyfrom(tempstr, strlen(tempstr)+1);
    		broadcast_send(&broadcast);
    		printf("broadcast message sent, tmp: %s\n\r",tempstr);
		
	}
	
  PROCESS_END();
}
