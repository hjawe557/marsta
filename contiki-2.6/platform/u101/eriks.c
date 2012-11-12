#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/process.h>
#include <sys/autostart.h>
#include <sys/etimer.h>
#include <usb-api.h>
#include <cdc-eth/cdc-eth.h>
#include <cdc-acm/cdc-acm.h>
#include <leds.h>
#if 0
#include <dhcps.h>
#endif
#include "i2c.h"
#include "pm.h"

/* Asynchronous */
#define USB_USER_MSG_TYPE_CONFIG 0x01 
#define USB_USER_MSG_TYPE_SUSPEND 0x02 
#define USB_USER_MSG_TYPE_RESUME 0x03

/* Synchronous, the supplied data is only valid during the event */
#define USB_USER_MSG_TYPE_EP_OUT(ep_addr) ((((ep_addr) & 0x7f)<<4) | 0x01)
#define USB_USER_MSG_TYPE_EP_IN(ep_addr) ((((ep_addr) & 0x7f)<<4) | 0x02)

struct usb_user_msg {
  unsigned int type;
  union {
    /* For EPx_OUT */
    unsigned short length;
    /* For CONFIG */
    unsigned char config;
  } data;
};

int until = 0;

#define ERIKS_PROCESS
#ifdef ERIKS_PROCESS
PROCESS(eriks_process, "eriks process");
PROCESS_THREAD(eriks_process, ev, data)
{
  static struct etimer timer;
  uint8_t buf[16];
  int res;
  volatile int dummy;
  int i;

  PROCESS_BEGIN();
  printf("eriks process\n\r");

  while(1) {

    etimer_set(&timer, CLOCK_SECOND * 1);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    printf("Hellu %d\n\r", until);
    until++;

    if ((until % 2) == 0) {
      leds_on(LEDS_ALL);
    } else {
      leds_off(LEDS_ALL);
    }
    
    if (until > 10) {
      leds_off(LEDS_ALL);
      pm_stop_mode(1);
    }
      
  }

  printf("Exiting eriks process\n\r");
  PROCESS_END();
}
#endif







#ifdef USBACM
PROCESS(usbacm_process, "USB ACM process");
PROCESS_THREAD(usbacm_process, ev , data)
{
  static struct etimer timer;
  uip_ipaddr_t myaddr;
  uip_ipaddr_t hostaddr;
  uip_ipaddr_t netmask;

  PROCESS_BEGIN();
  printf("USB ACM process\n\r");

  uip_init();

  /* usb_set_user_process(process_current); */

  uip_ipaddr(&myaddr, 10,0,0,2);
  uip_sethostaddr(&hostaddr);

  uip_ipaddr(&netmask, 255,0,0,0);
  uip_sethostaddr(&hostaddr);

  uip_ipaddr(&myaddr, 10,0,0,1);
  uip_ipaddr(&netmask, 255,255,0,0);

  usb_setup();

  usb_cdc_acm_setup();

  while(ev != PROCESS_EVENT_EXIT) {
    PROCESS_WAIT_EVENT();
    if (ev == PROCESS_EVENT_TIMER) {
      leds_toggle(LEDS_YELLOW);
      /* printf("FIFOP: %d\n", FIFOP_IS_1); */
      etimer_restart(&timer);
    } else if (ev == PROCESS_EVENT_MSG) {
      const struct usb_user_msg * const msg = data;
      switch(msg->type) {
      case USB_USER_MSG_TYPE_CONFIG:
        printf("User config\n");
        if (msg->data.config != 0) {
#if 0
          usb_setup_bulk_endpoint(DEV_TO_HOST,
                                  input_buffer, sizeof(input_buffer));
          usb_setup_bulk_endpoint(HOST_TO_DEV,
                                  output_buffer, sizeof(output_buffer));
          usb_setup_interrupt_endpoint(0x83,interrupt_buffer,
                                       sizeof(interrupt_buffer));
          etimer_set(&timer, CLOCK_SECOND);
#endif
        } else {
#if 0
          etimer_stop(&timer);
          usb_disable_endpoint(DEV_TO_HOST);
          usb_disable_endpoint(HOST_TO_DEV);
          usb_disable_endpoint(0x83);
#endif	  
        }
        break;
      case USB_USER_MSG_TYPE_EP_OUT(2):
        {
          /*unsigned int len = msg->data.length; 
            printf("Received %d:\n", len);  */
          {
            unsigned char ch;
            unsigned int xfer;
#if 0
            while((xfer = usb_recv_data(HOST_TO_DEV, &ch, 1)) > 0) {
              printf(" %02x",ch);
              /* if (slip_input_byte(ch)) break; */

            }
#endif
            /* printf("\n"); */
          }
        }
        break;
      }
    }
  }

  printf("Exiting USB ACM process\n\r");
  PROCESS_END();
}
#endif

