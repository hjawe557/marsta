#include <stdio.h>
#include "contiki.h"
#include "net/rime.h"
#include "dev/light-sensor.h"
#include "dev/leds.h"
#include "dev/cc2420.h"
#include "common.h"
#include <stdlib.h>
#include <io.h>

extern volatile uint16_t  precipitation;
//static uint16_t pulse_counter; //Added by Atena 
static uint16_t pulse_start_time; //Added by Atena 
static uint16_t pulse_end_time;  //Added by Atena
static uint16_t check; //Added by Atena 
static uint16_t duration;  //Added by Atena
static float period;  //Added by Atena
static  uint16_t  frequency;  //Added by Atena
PROCESS(sensor_process, "Sensor collect process");
AUTOSTART_PROCESSES(&sensor_process);

static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from){}

static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;
static struct etimer et;


static void send(uint16_t intensity1,uint16_t intensity2,enum pkt_type type,uint16_t seqno,rimeaddr_t sink_addr) {
  static uint8_t out_buf[128];
  struct pkt_format *pkt;
  uint16_t i;
  

  /* Pad the output buffer with the actual packet we're sending. */
  for (i=0;i<config.payload_len;i+=sizeof(struct pkt_format)) {
    pkt = (struct pkt_format *) (out_buf + i);
    pkt->type = type;
    pkt->seqno = seqno;
    pkt->sensor_readings=intensity1;
    pkt->sensor_readings2=intensity2;
    memcpy(&pkt->src, &rimeaddr_node_addr, sizeof(rimeaddr_t));
    //memcpy(&pkt->dest, dest, sizeof(rimeaddr_t));
  }

  packetbuf_copyfrom(out_buf, config.payload_len);
	unicast_send(&uc,&sink_addr);
}

PROCESS_THREAD(sensor_process, ev, data) {
  PROCESS_EXITHANDLER(unicast_close(&uc);)

  PROCESS_BEGIN();
  SENSORS_ACTIVATE(light_sensor);
  unicast_open(&uc, RIME_CHANNEL, &unicast_callbacks);
  static int16_t sensor_value_1;
  static int16_t sensor_value_2;
  static enum pkt_type type=SENSOR2;  //indicate what type of sensordata we are sending	
    P2SEL&= 0xf7;
    P2IE |= 0x00;
    P2DIR &= 0xf7; // Set P2.3 to input direction
   
  while(1) {
	  init();
    etimer_set(&et, CLOCK_SECOND*SAMPLING_PERIOD);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    sensor_value_1 = sensors_level();
    check= P2IN & BIT3;
    	  if (check==0x00){
    	  while (check==0x00){
    	  check= P2IN & BIT3;
    	}
    	  pulse_init();
    	      	  pulse_start_time = TBR;
    	      	 printf("in ISR \n");
    	      	  //pulse_counter=0;
    	      	check= P2IN & BIT3;

    	      	    	  while (check==0x08){
    	      	    	  check= P2IN & BIT3;
    	      	    	}
    	      	    	pulse_end_time = TBR;
    	      	    	    	 //pulse_end_time =((pulse_counter)*65536)+pulse_end_time;
    	      	    	    	duration= (pulse_end_time-pulse_start_time)*2;
    	      	    	    	//printf("Duration: %u\n",duration);
    	      	    	    	frequency=(1000000000)/(32768*duration);
    	      	    	    	//printf("Frequency: %u\n",frequency);
    	      	    	    	//leds_toggle(LEDS_ALL);
    	      	    	    	 pulse_start_time=0;
    	      	    	    	 pulse_end_time=0;
    	      	    	    	//TBCTL |= ~MC1;
    	      	    	    	//TBCTL |= ~MC0;
    	      	    	    	TBCTL=TBCLR;
    	  }
    	  else{
        	  while (check==0x08){
        	  check= P2IN & BIT3;
        	}
        	  pulse_init();
        	      	  pulse_start_time = TBR;
        	      	printf("in ISR \n");
        	      	 // pulse_counter=0;
        	      	check= P2IN & BIT3;

        	      	    	  while (check==0x00){
        	      	    	  check= P2IN & BIT3;
        	      	    	}
        	      	    	pulse_end_time = TBR;
        	      	    	    	 //pulse_end_time =((pulse_counter)*65536)+pulse_end_time;
        	      	    	    	duration= (pulse_end_time-pulse_start_time)*2;
        	      	    	    	//printf("Duration: %u\n",duration);
        	      	    	    	frequency=(1000000000)/(32768*duration);
        	      	    	    	//printf("Frequency: %u\n",frequency);
        	      	    	    	//leds_toggle(LEDS_ALL);
        	      	    	    	 pulse_start_time=0;
        	      	    	    	 pulse_end_time=0;
        	      	    	    	//TBCTL |= ~MC1;
        	      	    	    	//TBCTL |= ~MC0;
        	      	    	    	TBCTL=TBCLR;
    	  }
    sensor_value_2 = frequency;
    rimeaddr_t addr;
  	addr.u8[0] = RIME_SINK_0; //rime address of sink, byte0
  	addr.u8[1] = RIME_SINK_1; //rime address of sink, byte1
  	printf("send to addr %x:%x ", addr.u8[0],addr.u8[1]);
    send(sensor_value_1,sensor_value_2,type,config.seqno++,addr);
  }
	PROCESS_END();
}
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
void init(void)
{
  P6SEL |= 0x70; // ADC4 and ADC5 are used for light sensors and ADC6 is used for the level sensor.
  //P6DIR = 0xff;
  P6DIR = 0xbf;
  P6OUT = 0x00;

  /* Set up the ADC. */
  ADC12CTL0 = REF2_5V + SHT0_6 + SHT1_6 + MSC; // Setup ADC12, ref., sampling time
  ADC12CTL1 = SHP + CONSEQ_3 + CSTARTADD_0;	// Use sampling timer, repeat-sequenc-of-channels
  ADC12MCTL0 = (INCH_4 + SREF_0); // photodiode 1 (P64)
  ADC12MCTL1 = (INCH_5 + SREF_0); // photodiode 2 (P65)
  ADC12MCTL2 = (INCH_6 + SREF_0); // Level sensor (P66)

  ADC12CTL0 |= ADC12ON + REFON;

  ADC12CTL0 |= ENC;		// enable conversion
  ADC12CTL0 |= ADC12SC;		// sample & convert
}

/* The measured pressure */
int16_t sensors_level(void)
{
  return ADC12MEM2;
}

