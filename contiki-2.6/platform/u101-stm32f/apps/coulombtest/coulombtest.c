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
#include "stcn75.h"
#include <bq27200.c>

#include <leds.h>

/*---------------------------------------------------------------------------*/
PROCESS(coulombtest_process, "coulombtest");
AUTOSTART_PROCESSES(&coulombtest_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	
	printf("broadcast recive\r\n");
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(coulombtest_process, ev, data)
{
	uint16_t tmp;
	uint16_t volt = 0;
	static struct etimer et;
	PROCESS_BEGIN();

	broadcast_open(&broadcast, 129, &broadcast_call);
	bq27200_init();	//init i2C for bq27200 
	
	

	while(1) {
		etimer_set(&et, 3*CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		
		bq27200_read(&tmp,0x08,2);
		printf("Batt voltage: %dmV ",tmp);	

		/*bq27200_read(&tmp,0x16,2);
		printf("time to empty: %dmin (0x%x)\r\n",tmp);
		*/

		bq27200_read(&tmp,0x77,2);
		printf("EDV1&EDVF: 0x%x ",tmp);

		bq27200_read(&tmp,0x00,2);
		printf("MODE&CTRL: %x ",tmp);

		bq27200_read(&tmp,0x0A,1);
		printf("FLAGS: 0x%x\r\n",tmp);

		bq27200_read(&tmp,0x12,2);
		tmp = tmp/20;
		printf("LMD: %dmAh ",tmp);

		bq27200_read(&tmp,0x0E,2);
		tmp = tmp/20;
		printf("NAC: %dmAh ",tmp);

		bq27200_read(&tmp,0x14,2);
		tmp = tmp/20;
		printf("AI: %dmA ",tmp);

		bq27200_read(&tmp,0x1B,2);
		tmp = tmp/20;
		printf("SI: %dmA ",tmp);

		bq27200_read(&tmp,0x06,2);
		tmp = (tmp/4)-273;
		printf("temp: %dC\r\n\n",tmp);

		
	

		}	
	
  PROCESS_END();
}
