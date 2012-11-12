#ifndef CONTIKI_CONF_H
#define CONTIKI_CONF_H

#include <stdint.h>

#define CCIF
#define CLIF

#define WITH_UIP 1
#define WITH_ASCII 1

#define CLOCK_CONF_SECOND 100
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef int8_t s8_t;
typedef int16_t s16_t;
typedef int32_t s32_t;

typedef unsigned int clock_time_t;
typedef unsigned int uip_stats_t;

#ifndef BV
#define BV(x) (1<<(x))
#endif
/* #define UIP_CONF_LOGGING 1 */

#if WITH_UIP6
/* Network setup for IPv6 */
#define NETSTACK_CONF_NETWORK sicslowpan_driver
#define CXMAC_CONF_ANNOUNCEMENTS         0
#define XMAC_CONF_ANNOUNCEMENTS          0
#define UIP_CONF_ROUTER                  1
#ifndef UIP_CONF_IPV6_RPL
#define UIP_CONF_IPV6_RPL                1
#endif /* UIP_CONF_IPV6_RPL */

#else /* WITH_UIP6 */

#define NETSTACK_CONF_NETWORK     rime_driver

#define RIMEADDR_CONF_SIZE        2

#endif /* WITH_UIP6 */

/* Always */
#define NETSTACK_CONF_MAC         nullmac_driver
#define NETSTACK_CONF_RDC         nullrdc_driver
#define NETSTACK_CONF_FRAMER      framer_802154
#define NETSTACK_CONF_RADIO       rf230_driver


#define CHANNEL_802_15_4          26
/* Don't use the ARET stuff */
#define RF230_CONF_AUTORETRIES   0

/* uIP configuration */
#define UIP_CONF_LLH_LEN         0
#define UIP_CONF_BROADCAST       1
#define UIP_CONF_LOGGING         1
#define UIP_CONF_BUFFER_SIZE     1500

/* #define UIP_CONF_TCP_FORWARD     1 */
#define UIP_CONF_IP_FORWARD     1

/* Prefix for relocation sections in ELF files */
#define REL_SECT_PREFIX ".rel"

#define CC_BYTE_ALIGNED __attribute__ ((packed, aligned(1)))

#define USB_EP1_SIZE 64
#define USB_EP2_SIZE 64

#define RAND_MAX 0x7fff

/* Use alternate function of USART1 */
#define STM32_DBG_REMAP 0

/* RX and TX port name and number. These are interpreted as
   names by a macro. Don't quote or use parentheses. */
/* USART3 is closest to the CPU */
#define DBG_ON_USART3 1

#define LEDS_GREEN 1
#define LEDS_YELLOW 2
/* #define LEDS_BLUE 3 */
#define LEDS_RED 4

#endif /* __CONTIKI_CONF_H__CDBB4VIH3I__ */
