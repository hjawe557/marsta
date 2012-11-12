#include <stdio.h>
#include "contiki.h"
#include "net/rime.h"
#include "dev/light-sensor.h"
#include "dev/leds.h"
#include "dev/cc2420.h"
#include "common.h"

#include "dev/sht11-sensor.h"
#include "dev/light-sensor.h"


PROCESS(sensor_process, "Basestation sensor collect process");
PROCESS(sampling_process, "Sampling process");
AUTOSTART_PROCESSES(&sensor_process,&sampling_process);


//
// Sampling proccess at the basestation node to sample
// the sensor and print every 5 seconds
//
PROCESS_THREAD(sampling_process, ev, data) {
  PROCESS_BEGIN();
  static struct etimer et;
  static uint16_t light1, light2, temp, humid;
  static uint32_t seconds;
  static uint16_t now;

  SENSORS_ACTIVATE(light_sensor);
  SENSORS_ACTIVATE(sht11_sensor);

  etimer_set(&et, 0);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    etimer_set(&et, 5*CLOCK_SECOND);

    now = RTIMER_NOW();
    seconds = clock_seconds();

    light1 = light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC);
    light2 = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
    temp = sht11_sensor.value(SHT11_SENSOR_TEMP);
    humid = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);

    printf("secs=%lu, now=%u, light1=%u, light2=%u, temp=%u, humid=%u\n",
            seconds, now, light1, light2, temp, humid);
  }

  PROCESS_END();
}

//
// recieve unicast mesaage from one of the sensor nodes
//
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{
	uint8_t *ptr;
    uint16_t i;
    static struct pkt_format pkt;
    int16_t rssi, lqi;
    int8_t noise;
    uint8_t len, payload[128];

    leds_arch_set(LEDS_BLUE);
    ptr = packetbuf_dataptr();
    len = packetbuf_totlen();

    /* Parse the packet. */
    if (len >= 2) {
        pkt.type = ptr[0] | (ptr[1] << 8);
    }
    if (len >= 4) {
        pkt.seqno = ptr[2] | (ptr[3] << 8);
    }
    if (len >= 6) {
        pkt.src.u8[0] = ptr[4];
        pkt.src.u8[1] = ptr[5];
    }
    if (len >= 8) {
        pkt.sensor_readings = ptr[6] | (ptr[7] << 8);
    }
	if (len >= 10) {
        pkt.sensor_readings2 = ptr[8] | (ptr[9] << 8);
    }
    rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);
    lqi = packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY);
    noise = cc2420_rssi();
    memcpy(payload, ptr, len > 128 ? 128 : len);

    printf("recv: src=%02x:%02x, type=%hu, sensor=%u, sensor2=%u, seqno=%u, "
           "len=%hu, rssi=%d, lqi=%d, noise=%d, payload=(",
            pkt.src.u8[0], pkt.src.u8[1],
            pkt.type, pkt.sensor_readings,pkt.sensor_readings2,pkt.seqno, len, rssi, lqi, noise);

    /* Log payload. */
    for (i=0;i<(len > config.payload_len+2 ? config.payload_len+2 : len);i++) {
        printf("%x ", payload[i]);
    }
    printf("), \n");
	leds_arch_set(0);
    process_poll(&sensor_process);
}

static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;
static struct etimer et;

//
// main proccess, do nothing just wait for packet receptions
//
PROCESS_THREAD(sensor_process, ev, data) {

  PROCESS_BEGIN();
  unicast_open(&uc, RIME_CHANNEL, &unicast_callbacks);

  while(1) {
		PROCESS_WAIT_EVENT();
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_YIELD_UNTIL(etimer_expired(&et));
  }
  PROCESS_END();
}
