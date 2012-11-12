#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#define I2C_BUS_1  0
#define I2C_BUS_2  1

#define I2C_READ   1
#define I2C_WRITE  0

#define I2C_STOP   0
#define I2C_NOSTOP 1

void i2c_init(uint32_t bus, uint32_t rate);
int i2c_transact(uint32_t bus, uint8_t address, 
		 uint8_t rw, uint8_t len, 
		 uint8_t nostop,
		 uint8_t *buf);

int i2c_read(uint32_t bus, uint8_t addr, 
             const uint8_t len, uint8_t *buf);

int i2c_write(uint32_t bus, uint8_t addr, 
              const uint8_t len, uint8_t *buf,
              uint8_t nostop);

#endif
