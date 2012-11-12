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
#include "lmp.h"
#include <dev/lmp.c>

/*---------------------------------------------------------------------------*/
PROCESS(nodetest_process, "nodetest");
AUTOSTART_PROCESSES(&nodetest_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	//int temp;
	int rssi;
	int lqi;

	rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);
  	lqi = packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY);
	
  	printf("broadcast message received from %d.%d: '%s' RSSI: %d LQI: %d\r\n",
        	 from->u8[0], from->u8[1], (char *)packetbuf_dataptr(), rssi,lqi);

}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(nodetest_process, ev, data)
{
	uint16_t tmp;
	char tempstr[32];
	uint32_t adc = 0;
	const uint8_t gain = 32;

	static struct etimer et;
	PROCESS_BEGIN();
	
	broadcast_open(&broadcast, 129, &broadcast_call);
	printf("Initiating stcn75\r\n");
	stcn75_init();
	lmp_init();
	lmp_setup();
	
	while(1) {
		/* Delay 2-4 seconds */
    		etimer_set(&et, CLOCK_SECOND * 2 + random_rand() % (CLOCK_SECOND * 2));
		//etimer_set(&et, CLOCK_SECOND);		
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		
		//Reading ic temp
		stcn75_read(&tmp);

		//Reading probe temp
		if(lmp_read_data(ADC_DONE, 1) == 0xFF){
				printf("ADC data NOT Avlaiable\r\n");
			}else	{
				adc = lmp_read_data(ADC_DOUT, 3);	
			}

		//Converting ic tmp and RAWadc/gain data to string
		adc = adc/gain;
		adc = adc << 8;
		adc |= tmp;	
		snprintf(tempstr, sizeof(tempstr), "%d", adc);

		//Transmitting string
		packetbuf_copyfrom(tempstr, strlen(tempstr)+1);
    		broadcast_send(&broadcast);
    		printf("broadcast message sent, data: %s\n\r",tempstr);

// playing with RADIO settings...
//		printf("power: %d\r\n",rf230_get_txpower());		
	//printf("return: %d\r\n",rf230_cw_on());
		//rf230_cw_off();


		
	}
	
  PROCESS_END();
}
