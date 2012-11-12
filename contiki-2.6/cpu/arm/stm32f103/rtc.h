#ifndef RTC_H
#define RTC_H

#include "stm32f10x_map.h"
#include <stdint.h>
void rtc_sync(void);
void rtc_pre(void);
void rtc_post(void);
void rtc_set_alarm(uint32_t alarm);
void rtc_init(void);
void RTCAlarm_handler(void);
uint32_t rtc_read(void);

#endif
