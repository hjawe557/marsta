#include <stdio.h>
#include <string.h>

#include "dev/leds.h"
#include "uspi.h"
#include "contiki-conf.h"
#include <gpio.h>
#include <stm32f10x_nvic.h>

#include "em4325.h"

static inline void spi_em_cs_high()
{
  GPIOB->BSRR = 1<<12;
}

static inline void spi_em_cs_low()
{
  GPIOB->BRR = 1<<12;
}

uint16_t em_get_status(void)
{
  uint16_t data;
  spi_em_cs_low();

 (void)uspi_txrx(SPI_BUS_2, 0xe0);
  data = uspi_txrx(SPI_BUS_2,0xe0);

  spi_em_cs_high();

  return data;
}

//Läser från minnet
uint16_t em_read_word(uint8_t adr)
{
  uint16_t data=0;
  uint8_t tmp=0;
  spi_em_cs_low();
  
  (void)uspi_txrx(SPI_BUS_2, 0xE7);
  tmp = uspi_txrx(SPI_BUS_2, adr);

	while (tmp == 0){ //väntar in svaret från taggen...
		tmp = uspi_txrx(SPI_BUS_2, 0x00);
	}
       
 //Bygger ordet
  tmp = uspi_txrx(SPI_BUS_2, 0x00);
  data |= (tmp << 1*8);
  tmp = uspi_txrx(SPI_BUS_2, 0x00);
  data |= (tmp << 0*8);

  tmp = uspi_txrx(SPI_BUS_2, 0x00);
	if(tmp !=0){
		printf("Nåt är knas mer data finns...%x\r\n",tmp);
	}
  
  spi_em_cs_high();

  return data;
}

//Skriver till minnet
uint8_t em_write_word(uint8_t adr,uint16_t data)
{
  uint8_t stat=0;
  uint8_t tmp_lo,tmp_hi=0;

  //Delar upp ordet i två bytes.
  tmp_lo = data&0x00FF;
  tmp_hi = (data>>8)&0x00FF;

  spi_em_cs_low();
  
  (void)uspi_txrx(SPI_BUS_2, 0xE8);
  (void)uspi_txrx(SPI_BUS_2, adr);
  (void)uspi_txrx(SPI_BUS_2, tmp_hi);
  (void)uspi_txrx(SPI_BUS_2, tmp_lo);
 
	while (stat == 0){ //väntar in svaret från taggen...
		stat = uspi_txrx(SPI_BUS_2, 0x00);
	}
  
  spi_em_cs_high();

  return stat;
}

uint8_t em_get_sensors(uint32_t *sens, uint32_t *utc)
{
  uint8_t status = 0;
  uint32_t tmp;

  *sens = 0;
  *utc = 0;

  spi_em_cs_low();

  (void)uspi_txrx(SPI_BUS_2, 0xe4);

	while(status == 0){
  		status = uspi_txrx(SPI_BUS_2, 0x00);
	}

  tmp = uspi_txrx(SPI_BUS_2, 0x00);
  *sens |= (tmp << 3*8);
  tmp = uspi_txrx(SPI_BUS_2, 0x00);
  *sens |= (tmp << 2*8);
  tmp = uspi_txrx(SPI_BUS_2, 0x00);
  *sens |= (tmp << 1*8);
  tmp = uspi_txrx(SPI_BUS_2, 0x00);
  *sens |= (tmp << 0*8);

  tmp = uspi_txrx(SPI_BUS_2, 0x00);
  *utc |= (tmp << 3*8);
  tmp = uspi_txrx(SPI_BUS_2, 0x00);
  *utc |= (tmp << 2*8);
  tmp = uspi_txrx(SPI_BUS_2, 0x00);
  *utc |= (tmp << 1*8);
  tmp = uspi_txrx(SPI_BUS_2, 0x00);
  *utc |= (tmp << 0*8);

  spi_em_cs_high();

  return status;
}

uint16_t em_disable_transponder(void)
{
  uint16_t status=0;
	
  spi_em_cs_low();

  (void)uspi_txrx(SPI_BUS_2, 0xe2);
	while (status == 0){ //Väntar in svaret.. kan fastna i loop..
  	 status = uspi_txrx(SPI_BUS_2, 0x0);
	}
  spi_em_cs_high();

  return status;
}

uint16_t em_enable_transponder(void)
{
  uint16_t status=0;

  spi_em_cs_low();

  (void)uspi_txrx(SPI_BUS_2, 0xe3);
  while (status == 0){ //Väntar in svaret.. kan fastna i loop..
  	 status = uspi_txrx(SPI_BUS_2, 0x0);
	}

  spi_em_cs_high();

  return status;
}


void em_init(void)
{
  int i;
  uint8_t tmp;
  volatile uint8_t dummy;

  /* CS */
  GPIO_CONF_OUTPUT_PORT(B, 12, PUSH_PULL, 50);
  /* CLOCK */
  GPIO_CONF_OUTPUT_PORT(B, 13, ALT_PUSH_PULL, 50);
  /* MISO */
  GPIO_CONF_OUTPUT_PORT(B, 14, ALT_PUSH_PULL, 50);
  /* MOSI */
  GPIO_CONF_OUTPUT_PORT(B, 15, ALT_PUSH_PULL, 50);

  uspi_init(SPI_BUS_2, 7);

  spi_em_cs_high();

  for(i=0; i<100000; i++)
    dummy = i;

  tmp = em_get_status();
  printf("em_init done - status: %x\n\r", tmp);
}
