#ifndef EM4325_H
#define EM4325_H


void em_init(void);
uint16_t em_get_status(void);
uint16_t em_read_word(uint8_t adr);
uint8_t em_write_word(uint8_t adr,uint16_t data);
uint8_t em_get_sensors(uint32_t *sens, uint32_t *utc);
uint16_t em_disable_transponder(void);
uint16_t em_enable_transponder(void);



#endif
