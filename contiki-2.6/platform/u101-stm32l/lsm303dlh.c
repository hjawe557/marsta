#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/process.h>
#include <sys/autostart.h>
#include <sys/etimer.h>
#include <leds.h>
#include "i2c.h"
#include "lsm303dlh.h"


#define LSM303_PROCESS

int lsm303acc[3];
int lsm303mag[3];



/* We don't handle reading multiple bytes yet. */
int lsm303_read(uint8_t addr, uint8_t reg, uint8_t *data)
{
	uint8_t buf[16];
	int res;

	buf[0] = reg;

	/* Do almost a complete write (write address and the subregister
	   we want to read from) but don't send sthe I2C stop) */
	res = i2c_transact(I2C_BUS_1, addr, I2C_WRITE, 1, 
			   I2C_NOSTOP, buf);

	if (0 != res) {
		return -1;
	} 
	/* Then do a read from the device using the address and 
	   get the data returned from the device (subregister). */
	res = i2c_transact(I2C_BUS_1, addr, I2C_READ, 1, 
			   I2C_STOP, buf);
	if (0 != res) {
		return -2;
	}


	

	data[0] = buf[0];
	return 0;
}

int lsm303_write(uint8_t addr, uint8_t reg, uint8_t data)
{
	uint8_t buf[16];
	int res;

	buf[0] = reg;
	buf[1] = data;
	res = i2c_transact(/*I2C1_BASE*/I2C_BUS_1, addr, I2C_WRITE, 2, 
			   I2C_STOP, buf);

	if (0 != res) {
		return -3;
	}
	return 0;
}

int lsm303_get(uint8_t rawacc[6], uint8_t rawmag[6])
{
	int res;
	int i;

	for (i=0; i<6; i++) {
		res = lsm303_read(LSM303_I2CADDR_ACC, 
				  LSM303_ACCDATA_REG+i, 
				  &rawacc[i]);
		if (0 != res) {
			break;
		}
	}

	if (i<6) {
		return -1;
	}

	for (i=0; i<6; i++) {
		res = lsm303_read(LSM303_I2CADDR_MAG, 
				  LSM303_MAGDATA_REG+i, 
				  &rawmag[i]);
		if (0 != res) {
			break;
		}
	}

	if (i<6) {
		return -1;
	}


	return 0;
}

int lsm303_cook(uint8_t raw[6], int cooked[3], int shift)
{
        cooked[0] = (int) ((raw[1]) << 8) | raw[0];
        cooked[1] = (int) ((raw[3]) << 8) | raw[2];
        cooked[2] = (int) ((raw[5]) << 8) | raw[4];

	if (cooked[0] & 0x8000) { 
		cooked[0] = cooked[0] | 0xffff0000; 
	}
	if (cooked[1] & 0x8000) { 
		cooked[1] = cooked[1] | 0xffff0000; 
	}
	if (cooked[2] & 0x8000) { 
		cooked[2] = cooked[2] | 0xffff0000; 
	}

	cooked[0] >>= shift;
	cooked[1] >>= shift;
	cooked[2] >>= shift;
	return 0;
}

int lsm303_init(void)
{
	int res;
	uint8_t r;

	/* Init accelerometer */
	/* 5
	  res = lsm303_write(0x20, 0xa7);
	*/
	(void)lsm303_write(LSM303_I2CADDR_ACC, 0x20, 0x67);

	res = lsm303_read(LSM303_I2CADDR_ACC, 0x20, &r);
	if (0 == res) {
		printf("0x20: %x\n\r", r);
	} else {
		printf("Error: %d\n\r", res);
	}

	/* Init magnetometer */
	/* Data output rate 15Hz */
	(void)lsm303_write(LSM303_I2CADDR_MAG, 0x00, 0x04 << 2);
	/* +- 8.1 Gauss*/
	(void)lsm303_write(LSM303_I2CADDR_MAG, 0x01, 0x07 << 5);
	/* Continuous */
	(void)lsm303_write(LSM303_I2CADDR_MAG, 0x02, 0x00);

	return 0;
}


#ifdef LSM303_PROCESS
PROCESS(lsm303_process, "LSM303 process");
PROCESS_THREAD(lsm303_process, ev , data)
{
	static struct etimer timer;
	static int n;
	int res;
	int i;
	uint8_t rawacc[6];
	uint8_t rawmag[6];

	PROCESS_BEGIN();

	printf("lsm303 process\n\r");

	i2c_init(I2C_BUS_1, 0);

	for (i=0; i<0x1000; i++)
		;

	lsm303_init();

	n=0;
	
	while (1) {
		/* Delay */
		etimer_set(&timer, CLOCK_SECOND / 1);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

		res = lsm303_get(rawacc, rawmag);
		if (0 != res) {
			printf("Problem getting\n\r");
			break;
		}
		res = lsm303_cook(rawacc, lsm303acc, 4);
		if (0 != res) {
			printf("Problem cooking\n\r");
			break;
		}
		res = lsm303_cook(rawmag, lsm303mag, 4);
		if (0 != res) {
			printf("Problem cooking\n\r");
			break;
		}
#if 0
		if ((n % 10) == 0) {
			printf("(%d\tY,%d\tZ,%d)\n\r", 
			       lsm303acc[0], lsm303acc[1], lsm303acc[2]);
		}
#endif
		//leds_toggle(LEDS_ALL);
		n++;
	}	
	printf("Exiting lsm303 process\n\r");
	PROCESS_END();
}
#endif
