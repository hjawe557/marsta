#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/process.h>
#include <sys/autostart.h>
#include <sys/etimer.h>
#include <usb/usb-api.h>
#include <usb/cdc-eth/cdc-eth.h>
#include <leds.h>
#if 0
#include <dhcps.h>
#endif
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

PROCESS(eriks_process, "Eriks process");
PROCESS_THREAD(eriks_process, ev , data)
{
  static struct etimer timer;
  uip_ipaddr_t myaddr;
  uip_ipaddr_t hostaddr;
  uip_ipaddr_t netmask;
#if 0
  struct dhcps_client_lease dhleases[3];
  struct dhcps_config dhconfig;
#endif
  PROCESS_BEGIN();
  printf("Eriks super process\n\r");

  uip_init();

#if 0
  dhconfig.leases = &dhleases[0];
  dhconfig.num_leases = 3;
  dhconfig.default_lease_time = 3600;
  uip_ipaddr(&dhconfig.netmask, 255,255,255,255);
  uip_ipaddr(&dhconfig.dnsaddr, 1,2,3,4);
  uip_ipaddr(&dhconfig.dnsaddr, 1,0,0,1);
  dhconfig.flags = DHCP_CONF_NETMASK | DHCP_CONF_DEFAULT_ROUTER;
#endif
  /* usb_set_user_process(process_current); */

  uip_ipaddr(&myaddr, 10,0,0,2);
  uip_sethostaddr(&hostaddr);

  uip_ipaddr(&netmask, 255,0,0,0);
  uip_sethostaddr(&hostaddr);

  uip_ipaddr(&myaddr, 10,0,0,1);
  uip_ipaddr(&netmask, 255,255,0,0);

  usb_setup();
#if 0
  usb_cdc_eth_set_ifaddr(&myaddr);
  usb_cdc_eth_setup();
  
  dhcps_init(&dhconfig);
    
  while(ev != PROCESS_EVENT_EXIT) {
    PROCESS_WAIT_EVENT();
    printf("EVENT\n\r");
  }

#else
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
#if 0
          /*unsigned int len = msg->data.length; 
            printf("Received %d:\n", len);  */
          {
            unsigned char ch;
            unsigned int xfer;
	    /*
            while((xfer = usb_recv_data(HOST_TO_DEV, &ch, 1)) > 0) {
              printf(" %02x",ch);
	    */
              /* if (slip_input_byte(ch)) break; */

            }
#endif
            /* printf("\n"); */
        }
        break;
      }
    }
  }

#endif
  printf("Exiting USB process\n\r");
  PROCESS_END();
}


