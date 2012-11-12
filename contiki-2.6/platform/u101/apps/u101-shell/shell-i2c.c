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
 * $Id: shell-sensors.c,v 1.1 2010/10/25 09:03:39 salvopitru Exp $
 */
 
 /**
 * \file
 *         Shell function for temp and acc sensors.
 * \author
 *         Salvatore Pitrulli <salvopitru@users.sourceforge.net>
 */

#include <string.h>
#include <stdio.h>

#include "contiki.h"
#include "shell.h"
#include "contiki-net.h"

/*---------------------------------------------------------------------------*/
PROCESS(shell_i2c_process, "i2c");
SHELL_COMMAND(i2c_command,
	      "i2c",
	      "i2c: get sensor value",
	      &shell_i2c_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_i2c_process, ev, data)
{
  const char arg0[32];
  const char arg1[32];
  const char *next;

  PROCESS_BEGIN();

  if (data == NULL) {
    shell_output_str(&i2c_command,
		     "i2c: a sensor must be specified", "");
    PROCESS_EXIT();
  }

  next = strchr(data, ' ');
  if (next == data) {
    PROCESS_EXIT();
  }

  //arg1 = next + 1;

  printf("\r\n");
  printf("\r\n");
  printf("ARGS: [%s]\r\n", data);
  shell_output_str(&i2c_command, "i2c\r\n", "");
    

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
shell_i2c_init(void)
{
  shell_register_command(&i2c_command);
}
/*---------------------------------------------------------------------------*/
