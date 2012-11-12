/**
 * \file
 *         This application reads the data from UNIK 5000 pressure level sensor.
 * \author
 *         Atena
 */


/*#include "contiki.h"
#include "dev/leds.h"
#include "dev/level-sensor.h"
#include <stdio.h>
#include <string.h>
#include <dev/serial-line.h>
#include <stdint.h>
#include <etimer.h>
#include <sys/autostart.h>
#include <clock.h>


struct sky_collect_msg {
uint16_t level;
};

#define REXMITS 4


PROCESS(reading_level_process, "reading_level");
AUTOSTART_PROCESSES(&reading_level_process);

PROCESS_THREAD(reading_level_process, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
  while(1) {
    struct sky_collect_msg *msg;
    static struct etimer et;
    SENSORS_ACTIVATE(level_sensor);
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    leds_on(LEDS_ALL);
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    leds_off(LEDS_ALL);
    //msg = (struct sky_collect_msg *)packetbuf_dataptr();
    //packetbuf_set_datalen(sizeof(struct sky_collect_msg));
    msg->level = level_sensor.value(LEVEL_SENSOR_TOTAL);
    printf("The read value is #%i\n", msg ->level);
    SENSORS_DEACTIVATE(level_sensor);
  }

 exit:
  leds_off(LEDS_ALL);
  PROCESS_END();
}*/
/*---------------------------------------------------------------------------*/

/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
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
 * $Id: test-button.c,v 1.6 2010/02/03 09:54:38 joxe Exp $
 */

/**
 * \file
 *         An example of how to use the button and light sensor on
 *         the Tmote Sky platform.
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/level-sensor.h"
#include "dev/leds.h"


/*---------------------------------------------------------------------------*/
PROCESS(reading_level_process, "Reading level");
AUTOSTART_PROCESSES(&reading_level_process);
/*---------------------------------------------------------------------------*/
static uint8_t active;
PROCESS_THREAD(reading_level_process, ev, data)
{
  PROCESS_BEGIN();
  active = 0;
  SENSORS_ACTIVATE(button_sensor);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event &&
			     data == &button_sensor);
    leds_toggle(LEDS_ALL);
    if(!active) {
      /* activate light sensor */
      SENSORS_ACTIVATE(level_sensor);
      printf("Level: %d\n", level_sensor.value(2));
    } else {
      /* deactivate light sensor */
      printf("Level: %d\n", level_sensor.value(2));
      SENSORS_DEACTIVATE(level_sensor);
    }
    active ^= 1;
    leds_toggle(LEDS_ALL);
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

