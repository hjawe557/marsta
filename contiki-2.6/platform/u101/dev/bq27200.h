#ifndef BQ27200_H
#define BQ27200_H

int bq27200_init(void);
int bq27200_read(uint16_t *data, uint8_t reg, uint8_t len);
int bq27200_write(uint16_t *data, uint8_t adr);

#endif
