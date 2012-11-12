/*
 * Copyright (c) 2010, STMicroelectronics.
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
 */
 
 /**
 * \file
 *         Shell function for temp and acc sensors.
 * \author
 *         Salvatore Pitrulli <salvopitru@users.sourceforge.net>
 */

#include <string.h>
#include <stdio.h>
#include "stm32f10x_map.h"
#include "contiki.h"
#include "shell.h"
#include "contiki-net.h"
#include "spi.h"
#include "tn100.h"

struct shell_spi {
  uint32_t bus;
  uint32_t rate;
};

/*---------------------------------------------------------------------------*/
PROCESS(shell_spi_process, "spi");
SHELL_COMMAND(spi_command,
	      "spi",
	      "spi: toy with SPI",
	      &shell_spi_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_spi_process, ev, data)
{
  char arg0[32];
  char arg1[32];
  char arg2[32];
  const char *next;
  int len;
  uint32_t bus;

  PROCESS_BEGIN();

  if (data == NULL) {
    shell_output_str(&spi_command,
		     "spi: [bus#] [cmd] [args]", "");
    PROCESS_EXIT();
  }

  next = strchr(data, ' ');
  if (NULL == next) {
    PROCESS_EXIT();
  }

  len = (int)(next - (char *)data);
  memcpy(arg0, data, len);
  arg0[len] = '\0';

  switch(arg0[0]) {
  case 'r':
    printf("SPI read\r\n");
    break;
  case 'w':
    printf("SPI write\r\n");
    break;
  default:
    break;
  }

  next = strchr(next, ' ');
  if (next == NULL) {
    PROCESS_EXIT();
  }

  len = (int)(next - (char *)data);
  memcpy(arg1, data, len);
  arg1[len] = '\0';

  bus = shell_strtolong(bus, NULL);
  if (0 == bus) {
    printf("Bus 0\r\n");
    bus = SPI1_BASE;
  } else if (1 == bus) {
    printf("Bus 1\r\n");
    bus = SPI2_BASE;
  } else {
    printf("Bad bus\r\n");
    PROCESS_EXIT();
  }

  len = (int)(next - (char *)data);
  memcpy(arg2, data, len);
  arg2[len] = '\0';


  //(void)spi_txrx(SPI2_BASE, len);
  //data = spi_txrx(SPI2_BASE, 0x0);

  printf("\r\n");
  printf("\r\n");
  //printf("ARGS: [%s]\r\n", data);
  shell_output_str(&spi_command, "spi\r\n", "");
    

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
shell_spi_init(void)
{
  shell_register_command(&spi_command);
}
/*---------------------------------------------------------------------------*/
