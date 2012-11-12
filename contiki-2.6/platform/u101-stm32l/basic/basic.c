#include <stm32l.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <debug-uart.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <etimer.h>
#include <sys/autostart.h>
#include <clock.h>
#include "net/rime.h"
#include <net/netstack.h>
#include "lsm303dlh.h"
#include "stcn75.h"

/*---------------------------------------------------------------------------*/
PROCESS(basic_process, "basic");
AUTOSTART_PROCESSES(&basic_process);
/*---------------------------------------------------------------------------*/
static void abc_recv(struct abc_conn *c)
{
  printf("abc message received '%s'\n", (char *)packetbuf_dataptr());
}
static const struct abc_callbacks abc_call = {abc_recv};
static struct abc_conn abc;

PROCESS_THREAD(basic_process, ev, data)
{
  static struct etimer et;
  int len;

  PROCESS_EXITHANDLER(abc_close(&abc);)

  PROCESS_BEGIN();

  abc_open(&abc, 128, &abc_call);

  while(1) {
    static char buf[64];

    /* Delay 2-4 seconds */
    etimer_set(&et, CLOCK_SECOND / 1);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    snprintf(buf, sizeof(buf), "[%d,%d,%d]", 
	     lsm303acc[0], lsm303acc[1],lsm303acc[2]);
    len = strlen(buf);
    buf[len] = 0;

    printf("Johans process running!\r\n");
    printf("%d\r\n", len);
    packetbuf_copyfrom(buf, len+1);
    abc_send(&abc);
    printf("%s\r\n", buf);
  }

  PROCESS_END();
}

