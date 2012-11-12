#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/process.h>
#include <sys/autostart.h>
#include <sys/etimer.h>
#include <leds.h>
#include <i2c.h>
#include "stcn75.h"


int stcn75_init(void)
{
	/* Init i2c */
        i2c_init(I2C_BUS_1, 0);

	return 0;
}


int stcn75_read(uint16_t *temperature)
{
	uint8_t buf[16];
	int res;

        /* Read the temperature */
        res = i2c_read(I2C_BUS_1, 0x90, 2, buf);
        if (0 != res) {
          printf("Failed to read temperature (%d)\r\n", res);
          return -1;
        }

        *temperature = buf[0];
        
	return 0;
}
