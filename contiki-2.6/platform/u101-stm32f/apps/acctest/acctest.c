#include <stm32f10x_map.h>
#include <stm32f10x_dma.h>
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
#include <rf230bb.h>
#include "net/mac/frame802154.h"
#include "em4325.h"
#include "contiki.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "lsm303dlh.c"
#include <leds.h>
#include "stcn75.h"

/*---------------------------------------------------------------------------*/
PROCESS(acctest_process, "acctest");
AUTOSTART_PROCESSES(&acctest_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	
	printf("broadcast recive\r\n");
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(acctest_process, ev, data)
{
	uint16_t tmp = 0;
	static struct etimer et;
	int res;
	int i;
	uint8_t rawacc[6];
	uint8_t rawmag[6];
	PROCESS_BEGIN();
	stcn75_init();
	lsm303_init();
	broadcast_open(&broadcast, 129, &broadcast_call);
	
	

	while(1) {
		etimer_set(&et, 3*CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		
		
		
		
		res = lsm303_get(rawacc, rawmag);
		if (0 != res) {
			printf("Problem getting\n\r");
		}
		res = lsm303_cook(rawacc, lsm303acc, 4);
		if (0 != res) {
			printf("Problem cooking\n\r");
		}
		res = lsm303_cook(rawmag, lsm303mag, 4);
		if (0 != res) {
			printf("Problem cooking\n\r");
		}

			printf("acc(X %d Y,%d Z,%d)\n\r", lsm303acc[0], lsm303acc[1], lsm303acc[2]);
			//printf("mag(X %d Y,%d\05 Z,%d)\n\r", lsm303mag[0], lsm303mag[1], lsm303mag[2]);


		}	
	
  PROCESS_END();
}
