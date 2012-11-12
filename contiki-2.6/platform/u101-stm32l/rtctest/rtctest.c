#include <stm32l.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <sys/autostart.h>
#include <etimer.h>
#include <clock.h>
#include <leds.h>
//#include "rtc.h"
//#include "pm.h"
/*---------------------------------------------------------------------------*/
PROCESS(rtctest_process, "rtctest");
AUTOSTART_PROCESSES(&rtctest_process);
/*---------------------------------------------------------------------------*/


PROCESS_THREAD(rtctest_process, ev, data)
{
	static struct etimer et;
	PROCESS_BEGIN();
	printf("RTC init\r\n");
	rtc_init();
	//Alarm counter

	rtc_set_alarm(200);


	while(1) {
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));


		if (rtc_read() > 100 && rtc_read() < 150) {
			leds_off(LEDS_ALL);
			pm_stop_mode(0);
		}
		if (rtc_read() > 200) {
			leds_toggle(LEDS_ALL);
		}

		rtc_sync();
		printf("1s tick.. %d rtc alarm flag %d\r\n", rtc_read(), 				(RTC->CRL & RTC_CRL_ALRF));
		
	}
	
  PROCESS_END();
}

