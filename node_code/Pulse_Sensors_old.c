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

//extern volatile uint8_t pulse_counter; 
/*---------------------------------------------------------------------------*/
PROCESS(Pulse_Sensors_process, "Pulse_Sensors");
AUTOSTART_PROCESSES(&Pulse_Sensors_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(Pulse_Sensors_process, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
    P2SEL&= 0x7f;
    P2IE |= 0x80;
    P2DIR &= 0x7f; // Set P2.7 to input direction
    P2IE |= 0x80; // P2.7 interrupt enabled
    P2IES |= 0x80; // P2.7 Hi/lo edge
    P2IFG &= ~0x80; // P2.7 IFG cleared
    leds_on(LEDS_ALL);
    _BIS_SR(GIE); //interrupt
    while (1){
     _BIS_SR(LPM0_bits); 
  }
exit:
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
