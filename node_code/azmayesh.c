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
#include "dev/uart0.h"
#include "sys/energest.h"
#include "dev/watchdog.h"
#include "lib/ringbuf.h"


//extern volatile uint8_t pulse_counter; 
/*---------------------------------------------------------------------------*/
PROCESS(azmayesh_process, "Pulse_Sensors");
AUTOSTART_PROCESSES(&azmayesh_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(azmayesh_process, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
   UCA0CTL1 |= UCSWRST;            /* Hold peripheral in reset state */
  UCA0CTL1 |= UCSSEL_2;           /* CLK = SMCLK */

  UCA0BR0 = 0x45;                 /* 8MHz/115200 = 69 = 0x45 */
  UCA0BR1 = 0x00;
  UCA0MCTL = UCBRS_3;             /* Modulation UCBRSx = 3 */

  P3DIR &= ~0x20;                 /* P3.5 = USCI_A0 RXD as input */
  P3DIR |= 0x10;                  /* P3.4 = USCI_A0 TXD as output */
  P3SEL |= 0x30;                  /* P3.4,5 = USCI_A0 TXD/RXD */
  /*UCA0CTL1 &= ~UCSWRST;*/       /* Initialize USCI state machine */



  /* XXX Clear pending interrupts before enable */
  IFG2 &= ~UCA0RXIFG;
  IFG2 &= ~UCA0TXIFG;
  UCA0CTL1 &= ~UCSWRST;                   /* Initialize USCI state machine **before** enabling interrupts */
  IE2 |= UCA0RXIE;                        /* Enable UCA0 RX interrupt */
    leds_on(LEDS_ALL);
    _BIS_SR(GIE); //interrupt
    while (1){
     _BIS_SR(LPM0_bits); 
  }
exit:
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
