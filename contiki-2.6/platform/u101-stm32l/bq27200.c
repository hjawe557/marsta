#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/process.h>
#include <sys/autostart.h>
#include <sys/etimer.h>
#include <leds.h>
#include <i2c.h>
#include "bq27200.h"
#include "i2c.c"


int bq27200_init(void)
{
	/* Init i2c */
        i2c_init(I2C_BUS_1, 1); // 0 => ~250kHz, others => ~100kHz;

	return 0;
}

/* Ugly but working code to read specific registers..
	TODO: use Repeated Start instead, write a better code to read multiple registers.*/

int bq27200_read(uint16_t *data, uint8_t reg, uint8_t len)
{
	uint8_t buf[16];
	int res;
	bool nostop = false;

	buf[0]=reg;

	if(len!= 1){    //Keep it in bounce...    
	len = 2;
	}
	
	/* chose witch reg to read  */
	res = i2c_write(I2C_BUS_1, 0xAA, 1, buf, nostop);
        if (0 != res) {
          printf("I2C Failed to do something.. w (%d)\r\n", res);
          return -1;
        }
	
        *data = buf[0];
	*data = 0;//avoid bug..


        /* Read  */
        res = i2c_read(I2C_BUS_1, 0xAA, len, buf);
        if (0 != res) {
          printf("I2C Failed to do something.. r (%d)\r\n", res);
          return -1;
        }
	
	//2 bytes lsb comes first.
	if(len !=1){        
	*data = buf[1];
	*data = buf[0] | (*data << 8);
	}else *data = buf[0];

	return 0;
}

/* NOT TESTED!!!*/
int bq27200_write(uint16_t *data, uint8_t reg)
{
	uint8_t buf[16];
	int res;
	bool nostop = false;
	buf[0]=reg;
	//buf[1]=0xAA;
	
        /* write  */
	res = i2c_write(I2C_BUS_1, 0xAA, 1, buf, nostop);
        if (0 != res) {
          printf("I2C Failed to do something.. w (%d)\r\n", res);
          return -1;
        }
	
        *data = buf[0];

	return 0;
}
