#ifndef _COMMON_H
#define _COMMON_H

#define RIME_SINK_0 30
#define RIME_SINK_1 86
#define RIME_CHANNEL 155
#define CC2420_DEFAULT_CHANNEL 15
#define CC2420_DEFAULT_TXPOWER 31
#define DEFAULT_PAYLOAD_LEN 64  /* Must be at least 8 bytes. */
#define DEFAULT_PAYLOAD_LEN_VAISALA 128  /* Must be at least 8 bytes. */

#define DEFAULT_PERIOD 128      /* 128 = One second. */
#define SAMPLING_PERIOD 10     // in seconds
struct {
  uint8_t txpower;
  uint8_t channel;
  uint16_t seqno;
  int to_send;
  int period;
  uint8_t payload_len;
} config = { CC2420_DEFAULT_TXPOWER, CC2420_DEFAULT_CHANNEL, 0, 0,
             DEFAULT_PERIOD, DEFAULT_PAYLOAD_LEN };

enum pkt_type {
    SENSOR0,
    SENSOR1,  // Vaisala sensor
    SENSOR2,  // windsensor1, direction + speed
    SENSOR3,  // windsensor2, speed
    SENSOR4,  // precipitation sensor
    SENSOR5,  // level sensor
    SENSOR6
};

struct pkt_format {
  enum pkt_type type;
  uint16_t seqno;
  rimeaddr_t src;
  uint16_t sensor_readings1;
  uint16_t sensor_readings2;
  //rimeaddr_t dest;
};

#endif

