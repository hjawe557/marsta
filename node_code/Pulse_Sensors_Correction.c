#include "contiki.h"
#ifdef __IAR_SYSTEMS_ICC__
#include <msp430.h>
#else
#include <io.h>
#include <signal.h>
#endif
#include "dev/leds.h"
#include <stdlib.h>
#include <stdio.h>

static uint16_t pulse_counter; //Added by Atena 
static uint32_t pulse_start_time; //Added by Atena 
static uint32_t pulse_end_time;  //Added by Atena
static uint16_t check; //Added by Atena 
static uint64_t duration;  //Added by Atena
static float period;  //Added by Atena
static  uint64_t  frequency;  //Added by Atena

/*---------------------------------------------------------------------------*/
PROCESS(Pulse_Sensors_Correction_process, "Pulse_Sensors");
AUTOSTART_PROCESSES(&Pulse_Sensors_Correction_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(Pulse_Sensors_Correction_process, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
    P2SEL&= 0x7f;
    P2IE |= 0x00;
    P2DIR &= 0x7f; // Set P2.3 to input direction
    _BIS_SR(GIE); //interrupt
    while (1){
    	  check= P2IN & BIT7;
    	  if (check==0x00){
			   static struct etimer et;
			   etimer_set(&et,CLOCK_SECOND);
    	  while (check==0x00){
    	  check= P2IN & BIT7;
    	}
    	  pulse_init();
    	      	  pulse_start_time = TBR;
    	      	 printf("in ISR \n");
    	      	  pulse_counter=0;
    	      	check= P2IN & BIT7;

    	      	    	  while (check==0x80){
    	      	    	  check= P2IN & BIT7;
    	      	    	}
    	      	    	pulse_end_time = TBR;
    	      	    	    	 pulse_end_time =((pulse_counter)*65536)+pulse_end_time;
    	      	    	    	duration= (pulse_end_time-pulse_start_time)*2;
    	      	    	    	//printf("Duration: %u\n",duration);
    	      	    	    	frequency=(1000000000)/(32768*duration);
    	      	    	    	printf("Frequency: %u\n",frequency);
    	      	    	    	//leds_toggle(LEDS_ALL);
    	      	    	    	 pulse_start_time=0;
    	      	    	    	 pulse_end_time=0;
    	      	    	    	//TBCTL |= ~MC1;
    	      	    	    	//TBCTL |= ~MC0;
    	      	    	    	TBCTL=TBCLR;
    	  }
    	  else{
        	  while (check==0x80){
        	  check= P2IN & BIT7;
        	}
        	  pulse_init();
        	      	  pulse_start_time = TBR;
        	      	printf("in ISR \n");
        	      	  pulse_counter=0;
        	      	check= P2IN & BIT7;

        	      	    	  while (check==0x00){
        	      	    	  check= P2IN & BIT7;
        	      	    	}
        	      	    	pulse_end_time = TBR;
        	      	    	    	 pulse_end_time =((pulse_counter)*65536)+pulse_end_time;
        	      	    	    	duration= (pulse_end_time-pulse_start_time)*2;
        	      	    	    	//printf("Duration: %u\n",duration);
        	      	    	    	frequency=(1000000000)/(32768*duration);
        	      	    	    	printf("Frequency: %u\n",frequency);
        	      	    	    	//leds_toggle(LEDS_ALL);
        	      	    	    	 pulse_start_time=0;
        	      	    	    	 pulse_end_time=0;
        	      	    	    	//TBCTL |= ~MC1;
        	      	    	    	//TBCTL |= ~MC0;
        	      	    	    	TBCTL=TBCLR;
    	  }
    	static struct etimer et;
    	etimer_set(&et,10*CLOCK_SECOND );
     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }
exit:
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
pulse_init(void)
{
  // dint();
  /* start timer B - 32768 ticks per second */
  TBCTL = TBSSEL_1 | TBCLR;
  TBCTL |= TBIE;
  /* Start Timer_B in continuous mode. */
  TBCTL |= MC1;
  //TBR=0;
  printf("Timer initialized \n");
  TBR = RTIMER_NOW();
 // printf("Timer initialized \n");
  // eint();
}
