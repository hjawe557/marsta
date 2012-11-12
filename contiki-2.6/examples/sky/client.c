#include <stdio.h>
#include "contiki.h"
#include "net/rime.h"
#include "dev/light-sensor.h"
#include "dev/leds.h"
#include "dev/cc2420.h"
#include "common.h"
#include <stdlib.h>
#include <io.h>


PROCESS(sensor_process, "Test Light process");
AUTOSTART_PROCESSES(&sensor_process);

static void recv(struct broadcast_conn *con, const rimeaddr_t *sender) {
}

static struct etimer et;
static struct broadcast_conn bc;
static struct broadcast_callbacks bc_callback = { recv };

static void send(uint16_t intensity) {
	packetbuf_copyfrom(&intensity, sizeof(intensity));
	broadcast_send(&bc);
}

PROCESS_THREAD(sensor_process, ev, data) {
	PROCESS_BEGIN();
	static int16_t x;

	//SENSORS_ACTIVATE(light_sensor);
	broadcast_open(&bc, RIME_CHANNEL, &bc_callback);

	while (1) {
		 init();
                 x=sensors_level();
		leds_arch_set(LEDS_GREEN);
		send(x);
		leds_arch_set(0);

		etimer_set(&et,CLOCK_SECOND );
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	}

	PROCESS_END();
}

/*
 This is the driver for UNIK 5000 pressure level sensor.
 */
/*
 * Initialize periodic readings from the 2 photo diodes. The most
 * recent readings will be stored in ADC internal registers/memory.
 */
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
int
sensors_level(void)
{
  return ADC12MEM2;
}




