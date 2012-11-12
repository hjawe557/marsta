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
#include <leds.h>

/*---------------------------------------------------------------------------*/
PROCESS(gatewaytest_process, "gatewaytest");
AUTOSTART_PROCESSES(&gatewaytest_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	long temp;
	int rssi;
	int lqi;
	int i = 0;
	long RTD = 0;
	uint32_t hel = 0;
	uint32_t dec = 0;
	char tkn = ' ';
	uint16_t data[6];
	uint16_t tmp = 0;
	uint16_t res = 0;
	uint16_t nod_tmp = 0;
	uint16_t adr = 0x2c;

	rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);
  	lqi = packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY);

	switch (from->u8[0]){
	case 0xD0:		//Nod1
		adr=0x2c;
		break;
	case 0x4D:		//Nod2
		adr=0x33;
		break;
	case 0x1C:		//Nod3
		adr=0x3a;
		break;
	default :		//Nod?
		adr=0x2c;
		break;
	}

  /*	printf("broadcast message received from %d.%d: '%s'\r\n",
        	 from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
	printf("RSSI: %d LQI: %d \r\n",rssi,lqi);
*/
	temp = atoi((char *)packetbuf_dataptr());
	
	//Extract ic temp
	nod_tmp = temp & 0x000000FF;
	temp = temp >> 8;

	//Calculate probe temp
	RTD = (4*2010*temp)/(16777);
	if(RTD<100000) {
		temp = (100000-RTD)*25430;
		tkn = '-';
	}else{ 
		temp = (RTD-100000)*25700;
		tkn = ' ';
	}			
	hel = (temp/10000000);
	dec = ((temp-hel*10000000)/100000);
	//printf("RTD: %d temp: %d => %c%d.%02dC\r\n",RTD,temp,tkn,hel,dec);

	//Gatewaytemp	
	res = stcn75_read(&tmp);
	/*if (0 != res) {
		printf("Oh no!\r\n");	
	}*/
	
	//data words for rfid
	data[0] = tmp;
	data[1] = nod_tmp;
	data[2] = tkn;
	data[3] = hel;
	data[4] = dec;
	data[5] = rssi;
	data[6] = lqi;
 
	
/*
	printf("Broadcast from %03d.%03d: RSSI: %03d LQI: %03d recived '%s'\r\n", from->u8[0], from->u8[1], rssi, lqi, (char *)packetbuf_dataptr());
 	printf("Gatewaytemp %dC NodeIC_temp %dC NodeProbe_temp %c%d.%dC\r\n",tmp,nod_tmp,tkn,hel,dec);
*/
	//0x2C is the first physical adress in em4325 user memory.
	//printf("Writing 0x%x to mem adr 0x%x on em4325\r\n",data,0x2C);
	
	//status = F8 means command executed, device in sleep mode.
	//status = E0 means command executed, device in open mode....
	for(i=0; i<7; i++){
//em_write_word(adr,data[i]);
	printf("write status: %x\r\n",em_write_word(adr,data[i]));
	adr++;
	}
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(gatewaytest_process, ev, data)
{
	static struct etimer et;
	PROCESS_BEGIN();
	char tempstr[32];
	uint16_t tmp;
	broadcast_open(&broadcast, 129, &broadcast_call);
	printf("Initiating em4325\r\n");
	em_init();
	stcn75_init();

	while(1) {
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
//leds_toggle(LEDS_ALL);
		//read em mem
		//printf("read em status: %x\r\n",em_get_status());
		//printf("mem: %x ", em_read_word(0x2c));

		//write em mem
		//printf("write status: %x\r\n",em_write_word(0x2C,0xacab));

		

		//read gateway temp
		//stcn75_read(&tmp);
		//printf("onboard temp: %dC \r\n",tmp);

		//Send hello
	/*	packetbuf_copyfrom("Hello", 6);
    		broadcast_send(&broadcast);
    		printf("broadcast message sent\n\r");
	*/	
	}
	
  PROCESS_END();
}
