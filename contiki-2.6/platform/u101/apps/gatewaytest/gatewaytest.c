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

/*---------------------------------------------------------------------------*/
PROCESS(gatewaytest_process, "gatewaytest");
AUTOSTART_PROCESSES(&gatewaytest_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	int temp;
	uint16_t data = 0;
	uint16_t tmp = 0,res = 0;
	uint8_t t_flag, nod_id;
	

  	printf("broadcast message received from %d.%d: '%s'\r\n",
        	 from->u8[0], from->u8[1], (char *)packetbuf_dataptr());

	temp = atoi((char *)packetbuf_dataptr());

	//Setting temperatur flag
	if (temp>30){
		t_flag=3;		//Temp hi
	}else if(temp<25){
		t_flag=2;		//Temp lo
	}else{
		t_flag=1;		//Temp normal
	}
	
	//Finding out witch one	
	if (from->u8[0]==0x34){
		nod_id=3;		//Node nr 3
	}else if(from->u8[0]==0x02){
		nod_id=2;		//Node nr 2
	}else if(from->u8[0]==0x28){
		nod_id=6;		//Node nr 6
	}else if(from->u8[0]==0xe7){
		nod_id=10;		//Node nr 10
	}
	
	//Build data word
	tmp = temp;
	tmp = tmp << 4;
	tmp |= t_flag;
	tmp = tmp << 4;
	tmp |= nod_id;
  	data = tmp;
	
	res = stcn75_read(&tmp);
	if (0 != res) {
		printf("Oh no!\r\n");	
	}
	printf("Gatewaytemp %dC Nodetemp %dC\r\n",tmp,temp);

	//0x2C is the first physical adress in em4325 user memory.
	printf("Writing 0x%x to mem adr 0x%x on em4325\r\n",data,0x2C);
	
	//status = F8 means command executed, device in sleep mode.
	printf("write status: %x\r\n",em_write_word(0x2C,data));

}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(gatewaytest_process, ev, data)
{
	static struct etimer et;
	PROCESS_BEGIN();
	
	broadcast_open(&broadcast, 129, &broadcast_call);
	printf("Initiating em4325\r\n");
	em_init();
	stcn75_init();

	while(1) {
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		//Send hello
		packetbuf_copyfrom("Hello", 6);
    		broadcast_send(&broadcast);
    		printf("broadcast message sent\n\r");
		
	}
	
  PROCESS_END();
}
