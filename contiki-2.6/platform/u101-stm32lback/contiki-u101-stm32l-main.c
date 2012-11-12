#include <gpio.h>
/* #include <nvic.h> */
#include <stdint.h>
#include <stdio.h>
#include <debug-uart.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <etimer.h>
#include <sys/autostart.h>
#include <clock.h>
#include <usb-api.h>

#include <dev/leds.h>
#include <dev/serial-line.h>
#include <net/mac/sicslowmac.h>
#include <net/rime.h>
#include <net/netstack.h>
#include <rf230bb.h>
#include <net/mac/frame802154.h>
#include <tcpip.h>
#include <cdc-eth/cdc-eth.h>
#ifdef WITH_UIP
#include <net/uip.h>
#include <net/uip-fw.h>
#include <net/uip-fw-drv.h>
#include <net/uip-over-mesh.h>
#endif
#if WITH_UIP6
#include <net/uip-ds6.h>
#include <uip-debug.h>
#endif /* WITH_UIP6 */
/* #include "em4325.h" */
/* #include "eriks.h" */
#include "lsm303dlh.h" 
/* #include "pm.h" */

unsigned int idle_count = 0;

#ifdef WITH_USB
PROCESS_NAME(usbeth_process);
#endif

#define U101_RF231 1

static void
print_local_addresses(void)
{
#if defined WITH_UIP6
  int i;
  uint8_t state;

  printf("Server IPv6 addresses: \n");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(state == ADDR_TENTATIVE || state == ADDR_PREFERRED) {
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      printf("\n");
      /* hack to make address "final" */
      if (state == ADDR_TENTATIVE) {
        uip_ds6_if.addr_list[i].state = ADDR_PREFERRED;
      }
    }
  }
#endif
}


void setup_usb(void)
{
  uip_ipaddr_t hostaddr, netmask, defrouter;
  
  uip_ipaddr(&hostaddr, 10,0,0,2);
  uip_ipaddr(&netmask, 255,0,0,0);
  uip_ipaddr(&defrouter, 10,0,0,1);
  
  uip_sethostaddr(&hostaddr);
  uip_setnetmask(&netmask);
  uip_setdraddr(&defrouter);
  /*
    uip_over_mesh_set_net(&hostaddr, &netmask);
    uip_fw_register(&slipif);
    uip_over_mesh_set_gateway_netif(&slipif);
    uip_fw_default(&meshif);
    uip_over_mesh_init(UIP_OVER_MESH_CHANNEL);
  */
  printf("uIP started with IP address %d.%d.%d.%d\n",
         uip_ipaddr_to_quad(&hostaddr));

#ifdef WITH_USB
  printf("Setting up USB\n");
  usb_setup();
  printf("usb setup done\n");
  usb_cdc_eth_set_ifaddr(&hostaddr);
  usb_cdc_eth_setup();
#endif
  printf("cdc_eth setup done\n");
}

static uint16_t
u101_chksum(uint16_t sum, const u8_t *data, u16_t len)
{
  u16_t t;
  const u8_t *dataptr;
  const u8_t *last_byte;

  dataptr = data;
  last_byte = data + len - 1;
  
  while(dataptr < last_byte) {  /* At least two more bytes */
    t = (dataptr[0] << 8) + dataptr[1];
    sum += t;
    if(sum < t) {
      sum++;            /* carry */
    }
    dataptr += 2;
  }
  
  if(dataptr == last_byte) {
    t = (dataptr[0] << 8) + 0;
    sum += t;
    if(sum < t) {
      sum++;            /* carry */
    }
  }

  /* Return sum in host byte order. */
  return sum;
}

void lowlevel_init()
{
  rimeaddr_t rimeaddr;
  uint16_t *fsize = (uint16_t *)0x1FF8004C;
  uint16_t *uid96 = (uint16_t *)0x1FF80050;
  uint32_t *dbgmcu_id = (uint32_t *)0xE0042000;
  uint16_t uid16;

  dbg_setup_uart();

  printf("\nInitialising\n");

  printf("Device ID: 0x%03x, silicon rev: 0x%04x\n", 
         (unsigned int)*dbgmcu_id & 0x0fff, 
         (unsigned int)(*dbgmcu_id >> 16) & 0xffff);
  printf("Flash size is %d kB\n", *fsize);
  printf("UID96 is %04x %04x %04x %04x %04x %04x\n", 
         uid96[0], uid96[1], uid96[2], uid96[3], uid96[4], uid96[5]);
  uid16 = u101_chksum(0, (uint8_t *)uid96, 12);
  printf("Pseudo-UID16 is %02x\n", uid16);

  clock_init();
  rtimer_init();
  process_init();
  process_start(&etimer_process, NULL);
  ctimer_init();
  serial_line_init();
  leds_init();

#ifdef U101_RF231
  printf("Low-level networking init\n");
  queuebuf_init();
  NETSTACK_RADIO.init();
  NETSTACK_RADIO.on();
  NETSTACK_MAC.init();
  NETSTACK_RDC.on();
#endif
#if 0
  printf("%s %s, channel check rate %u Hz, radio channel %u\n",
         NETSTACK_MAC.name, 
         NETSTACK_RDC.name,
         CLOCK_SECOND / (NETSTACK_RDC.channel_check_interval() == 0 ? 1:
                         NETSTACK_RDC.channel_check_interval()),
         RF_CHANNEL);
#endif
  //memcpy(&uip_lladdr.addr, ds2411_id, sizeof(uip_lladdr.addr));

#if defined WITH_UIP6

  printf("\nAddresses [%u max]\n", UIP_DS6_ADDR_NB);
  
  for (i=0; i<UIP_DS6_ADDR_NB; i++) {
    if (uip_ds6_if.addr_list[i].isused) {
      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
      printf("\n");
    }
  }
#endif

  /* Temporarily, we use a part of the STM32's UID as address. 
     It seems like uid_0[1] is usable in our batch. Note that
     this does not guarrantee unique addresses.
   */
  rimeaddr.u8[0] = (uint8_t)(uid16 >> 8) & 0xff;
  rimeaddr.u8[1] = (uint8_t)(uid16 & 0xff);
  printf("Rime address is: %02x.%02x\n", 
         rimeaddr.u8[0], rimeaddr.u8[1]);

#if NETSTACK_CONF_RADIO == rf230_driver
  rf230_set_pan_addr(IEEE802154_PANID, 0, (uint8_t *)&rimeaddr.u8);
  rf230_set_channel(CHANNEL_802_15_4);
  rimeaddr_set_node_addr(&rimeaddr);
#endif
  process_start(&tcpip_process, NULL);

#if defined WITH_UIP6

  printf("Tentative link-local IPv6 address ");
  {
    uip_ds6_addr_t *lladdr;
    int i;
    lladdr = uip_ds6_get_link_local(-1);
    for(i = 0; i < 7; ++i) {
      printf("%02x%02x:", lladdr->ipaddr.u8[i * 2],
             lladdr->ipaddr.u8[i * 2 + 1]);
    }
    printf("%02x%02x\n", lladdr->ipaddr.u8[14], lladdr->ipaddr.u8[15]);
  }
  printf("\n");
#endif

  //em_init();

  leds_on(LEDS_ALL);

  print_local_addresses();

#ifdef WITH_UIP
  printf("Starting tcpip and fw\n");
  process_start(&tcpip_process, NULL);
  process_start(&uip_fw_process, NULL);
#endif
#ifdef WITH_USB
  //(void)setup_usb();
  process_start(&usbeth_process, NULL);
#endif
  //process_start(&lsm303_process, NULL);
  //process_start(&eriks_process, NULL);
  printf("Processes running\n");
}




int main()
{
  unsigned char ser_byte;
  int res;

  /* Don't do ANYTHING before this. */
  lowlevel_init();

  leds_off(LEDS_ALL);

  //pm_stop_mode(0);

  uip_init();

  autostart_start(autostart_processes);

  while(1) {
    do {
    } while(process_run() > 0);
    idle_count++;
    
    if (dbg_getbyte(&ser_byte)) {
      //leds_toggle(LEDS_ALL);
      printf("%c", ser_byte);
      res = serial_line_input_byte(ser_byte);
      if (!res) {
	printf("No?\n");
	/* Well... */
      }
    }


    /* Idle! */
    /* Stop processor clock */
    //asm("wfi"::);
  }
  return 0;
}
