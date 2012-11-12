/*
 This is the driver for UNIK 5000 pressure level sensor.
 */


#include "contiki.h"
#include <stdlib.h>
#include "lib/sensors.h"
#include "dev/sky-sensors.h"
#include <io.h>
#include <stdio.h>
#include "dev/button-sensor.h"
#include "dev/leds.h"

/*---------------------------------------------------------------------------*/
PROCESS(test_process, "Reading level");
AUTOSTART_PROCESSES(&test_process);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(test_process, ev, data)
{
  PROCESS_BEGIN();
  while(1) {
    static struct etimer et;
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
      init();
      printf("Level: %d\n", sensors_level());
    leds_toggle(LEDS_ALL);
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/



/*
 * Initialize periodic readings from the 2 photo diodes. The most
 * recent readings will be stored in ADC internal registers/memory.
 */
void
init(void)
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




