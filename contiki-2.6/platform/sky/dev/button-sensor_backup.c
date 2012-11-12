/*
 * Copyright (c) 2005, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * @(#)$Id: button-sensor.c,v 1.4 2010/01/14 20:01:19 nifi Exp $
 */
#include "lib/sensors.h"
#include "dev/hwconf.h"
#include "dev/button-sensor.h"
#include <signal.h>
#include "dev/leds.h"

const struct sensors_sensor button_sensor;

static struct timer debouncetimer;
static int status(int type);

static uint16_t pulse_counter; //Added by Atena 
static uint32_t pulse_start_time; //Added by Atena 
static uint32_t pulse_end_time;  //Added by Atena
static uint16_t check; //Added by Atena 
static uint64_t duration;  //Added by Atena
static float period;  //Added by Atena
static  uint64_t  frequency;  //Added by Atena
HWCONF_PIN(BUTTON, 2, 7);
HWCONF_IRQ(BUTTON, 2, 7);

/*---------------------------------------------------------------------------*/
interrupt(PORT2_VECTOR)
     irq_p2(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);

  if(BUTTON_CHECK_IRQ()) {
    if(timer_expired(&debouncetimer)) {
      timer_set(&debouncetimer, CLOCK_SECOND / 4);
      sensors_changed(&button_sensor);
      LPM4_EXIT;
    }
  }
//Added by Atena as the ISR for P2.7 which will be connected to any sensor with pulse output
//*****************************************************************************************
  if(P2IFG & BIT7){
   //dint();
  P2IE &= 0x7f; 
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
printf("Duration: %u\n",duration);
frequency=(1000000000)/(32768*duration);
printf("Frequency: %u\n",frequency);
//leds_toggle(LEDS_ALL);
 pulse_start_time=0;
 pulse_end_time=0;
//TBCTL |= ~MC1;
//TBCTL |= ~MC0;
TBCTL=TBCLR;
  }
/*if(P2IFG & BIT3){
   //dint();
  P2IE &= 0xf7; 
  pulse_init();
  pulse_start_time = TBR;
  printf("in ISR \n");
  pulse_counter=0;
  check= P2IN & BIT3;
  while (check==0x00){
  check= P2IN & BIT3;
} 
 pulse_end_time = TBR;
 pulse_end_time =((pulse_counter)*65536)+pulse_end_time;
duration= (pulse_end_time-pulse_start_time)*2;
printf("Duration: %u\n",duration);
frequency=(1000000000)/(32768*duration);
printf("Frequency: %u\n",frequency);
//leds_toggle(LEDS_ALL);
 pulse_start_time=0;
 pulse_end_time=0;
//TBCTL |= ~MC1;
//TBCTL |= ~MC0;
TBCTL=TBCLR;
  }*/
//*****************************************************************************************
 P2IFG = 0x00;
 P2IE |= 0x80; //Atena 
 // P2IE |= 0x08; //Atena 
 //eint();       //Atena
  //TBCCR1=0;  //Atena
  //TBR=0;     //Atena
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}
/*---------------------------------------------------------------------------*/
static int
value(int type)
{
  return BUTTON_READ() || !timer_expired(&debouncetimer);
}
/*---------------------------------------------------------------------------*/
static int
configure(int type, int c)
{
  switch (type) {
  case SENSORS_ACTIVE:
    if (c) {
      if(!status(SENSORS_ACTIVE)) {
	timer_set(&debouncetimer, 0);
	BUTTON_IRQ_EDGE_SELECTD();

	BUTTON_SELECT();
	BUTTON_MAKE_INPUT();

	BUTTON_ENABLE_IRQ();
      }
    } else {
      BUTTON_DISABLE_IRQ();
    }
    return 1;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
status(int type)
{
  switch (type) {
  case SENSORS_ACTIVE:
  case SENSORS_READY:
    return BUTTON_IRQ_ENABLED();
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
//Added by Atena as the ISR for P2.7 which will be connected to any sensor with pulse output
//*****************************************************************************************
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
  //printf("Timer initialized \n");
  TBR = RTIMER_NOW();
  printf("Timer initialized \n");
  // eint();
}
/*---------------------------------------------------------------------------*/
/* Timer B1 ISR */
#ifdef __IAR_SYSTEMS_ICC__
#pragma vector=TIMERB1_VECTOR
__interrupt void
#else
interrupt(TIMERB1_VECTOR)
#endif
timerb1_interrupt(void)
{
  int tbiv;
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
    //watchdog_start();
  /* always read TBIV to clear IFG */
  printf("Timer overflowed \n");
  tbiv = TBIV;
    pulse_counter++;
    //watchdog_stop();
    TBIV=0;
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}
/*---------------------------------------------------------------------------*/
//*****************************************************************************************
/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(button_sensor, BUTTON_SENSOR,
	       value, configure, status);
