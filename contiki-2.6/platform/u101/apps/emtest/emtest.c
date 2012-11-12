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

/*---------------------------------------------------------------------------*/
PROCESS(emtest_process, "emtest");
AUTOSTART_PROCESSES(&emtest_process);
/*---------------------------------------------------------------------------*/


PROCESS_THREAD(emtest_process, ev, data)
{
	static struct etimer et;
	static int state = 0;
	PROCESS_BEGIN();
	
	while(1) {
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		if (state == 0) {
			printf("Initierar & testar olika kommandon\r\n");
			em_init();

		} else if (state == 2){
			printf("idle...\r\n");


		}else if (state == 3) { // Läser minnet
			uint16_t data;
			uint8_t adr=0x2c;
			uint8_t end_adr=0x2c;
			
			printf("Reading from physical mem adr: %x-%x Data Word(s): 						\r\n",adr,end_adr);
			while(adr<=end_adr){
			data = em_read_word(adr);
			printf("%x ",data);
			adr++;
			}
			printf("\r\n");
			
			//stat = em_get_status();
			//printf("status: %x\r\n", stat);

		}/*else if (state == 5){//Skriver, TÄNK FÖRST!!!		
			uint8_t stat = 0;
			uint8_t adr = 0x2C;// 2C = första adr i user mem.
			uint8_t end_adr = 0x2C;// EB = sista adr i user mem.
			uint16_t data = 0xDA7A;

			printf("Writing %x to mem adr %x-%x\r\n",data,adr,end_adr);

				//while(adr<=end_adr){
				//	stat = em_write_word(adr,data);
				//	adr++;
				//}
			
			stat = em_write_word(adr,data);
			printf("write status: %x\r\n",stat);




		} else if (state == 7) {
			uint32_t sens;
			uint32_t utc;
			uint16_t stat;
			stat = em_get_sensors(&sens, &utc);
			printf("status: %x, sens: %x, utc: %x\r\n", 
			       stat, (unsigned int)sens, (unsigned int)utc);
			
		} else if (state == 9) {	//Dis Transp

			uint16_t stat;
			stat = em_disable_transponder();
			printf("Disabled transponder (%x)\r\n", stat);
			//stat = em_get_status();
			//printf("status: %x\r\n", stat);

		} else if (state == 13) {	//En Transp

			uint8_t stat;
			stat = em_enable_transponder();
			printf("Enabled transponder (%x)\r\n", stat);
			//stat = em_get_status();
			//printf("status: %x\r\n", stat);
		}*/

		state++;
		if (state > 15) {
			state = 2;
		}
	}
	
  PROCESS_END();
}

