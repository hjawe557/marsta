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
#include "lmp.h"
#include <lmp.h>

/*---------------------------------------------------------------------------*/
PROCESS(lmptest_process, "lmptest");
AUTOSTART_PROCESSES(&lmptest_process);
/*---------------------------------------------------------------------------*/


PROCESS_THREAD(lmptest_process, ev, data)
{
	static struct etimer et;
	const int bits = 16777216; //2^24
	const int bits2 = bits/1000;
	uint32_t ADC = 0;
	long RTD = 0;
	uint32_t temp = 0;
	uint32_t hel = 0;
	uint32_t dec = 0;
	char tkn = ' ';
	const uint8_t gain = 32;
	

	PROCESS_BEGIN();

	printf("Initierar & testar\r\n");
	lmp_init();
	lmp_setup();

	while(1) {
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	
			// read adress and nr of bytes to receve
			printf("CH_STS: %x",lmp_read_data(CH_STS, 1));
			printf(", ADC data: ");
			if(lmp_read_data(ADC_DONE, 1) == 0xFF){
				printf("NOT Avlaiable\r\n");
			}else	{
				ADC = lmp_read_data(ADC_DOUT, 3);
				printf("%u ",ADC);		
			}
			

			/*/OBS Massa trunkeringsfel...
			RTD = (8000*ADC)/(bits);
			temp = (RTD-100)*2.59699787; 			
			printf("ADC_Hex: %x, RTD: %d, temp: %dC\r\n ",ADC,RTD,temp);
		
			RTD = 0;
			temp = 0;
*/

			//mindre trunkfel
			ADC = ADC/gain;
			RTD = (4*2020*ADC)/(bits2);
			if(RTD<100000) {
				temp = (100000-RTD)*25430;
				tkn = '-';
			}else{ 
				temp = (RTD-100000)*25700;
				tkn = ' ';
			}			
			hel = (temp/10000000);
			dec = ((temp-hel*10000000)/100000);
			printf("RTD: %d temp: %d => %c%d.%02dC\r\n",RTD,temp,tkn,hel,dec);

	

			//printf(" %x, %x, %x\r\n",lmp_read_data(ADC_DOUTH,1),lmp_read_data(ADC_DOUTM,1),lmp_read_data(ADC_DOUTL,1));

	/*		printf("CH0_INPUTCH:%x, CH0_CONFIG:%x, SCALCN:%x, BGCALCN:%x, ",lmp_read_data(CH0_INPUTCN,1),lmp_read_data(CH0_CONFIG,1),lmp_read_data(SCALCN,1),lmp_read_data(BGCALCN,1));
			printf("ADC_AUXCN:%x, CH_STS:%x, CH_SCAN:%x\r\n",lmp_read_data(ADC_AUXCN,1),lmp_read_data(CH_STS,1),lmp_read_data(CH_SCAN,1));
	*/
			/*printf("SENSOR diag flags: %x",lmp_read_data(SENDIAG_FLAGS,1));
			printf(" adc restart\r\n");
			lmp_write_data(ADC_RESTART,0x01);*/
	}
	
  PROCESS_END();
}

