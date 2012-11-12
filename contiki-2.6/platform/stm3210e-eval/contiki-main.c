#include <stm32f10x_map.h>
#include <stm32f10x_dma.h>
#include <gpio.h>
#include <nvic.h>
#include <stdint.h>
#include <stdio.h>
#include <debug-uart.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <etimer.h>
#include <sys/autostart.h>
#include <clock.h>
#include <usb/usb-api.h>
#include <usb/cdc-acm/cdc-acm.h>
#include <dev/leds.h>
#include <dev/serial-line.h>
#include <cdc-eth/cdc-eth.h>

#if WITH_UIP
#include "net/uip.h"
#include "net/uip-fw.h"
#include "net/uip-fw-drv.h"
#include "net/uip-over-mesh.h"

/*
static struct uip_fw_netif slipif =
  {UIP_FW_NETIF(192,168,1,2, 255,255,255,255, slip_send)};
static struct uip_fw_netif meshif =
  {UIP_FW_NETIF(172,16,0,0, 255,255,0,0, uip_over_mesh_send)};
static struct uip_fw_netif usbethif =
  {UIP_FW_NETIF(10,0,0,2, 255,255,255,255, usbeth_send)};
*/

#endif /* WITH_UIP */

unsigned int idle_count = 0;


int
main()
{
  unsigned char ser_byte;
  int res;

  dbg_setup_uart();
  printf("Initialising\n\r");

  clock_init();
  leds_init();

  /* leds_on(LEDS_GREEN); */

#if WITH_UIP
  process_init();
  process_start(&etimer_process, NULL);
  serial_line_init();

  process_start(&tcpip_process, NULL);
  process_start(&uip_fw_process, NULL);
  /* process_start(&slip_process, NULL); */

  {
    uip_ipaddr_t hostaddr, netmask, defrouter;

    uip_init();

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

    usb_setup();
    printf("usb setup done\n\r");
    
    usb_cdc_eth_set_ifaddr(&hostaddr);
    usb_cdc_eth_setup();
    printf("cdc_eth setup done\n\r");
  }
#endif /* WITH_UIP */

  autostart_start(autostart_processes);

  printf("Processes running\n\r");
  while(1) {
    do {
    } while(process_run() > 0);
    idle_count++;
    
    if (dbg_getbyte(&ser_byte)) {
      printf("%c", ser_byte);
      res = serial_line_input_byte(ser_byte);
      if (!res) {
	printf("No?\r\n");
	/* Well... */
      }
    }

    /* Idle! */
    /* Stop processor clock */
    /* asm("wfi"::); */ 
  }
  return 0;
}
