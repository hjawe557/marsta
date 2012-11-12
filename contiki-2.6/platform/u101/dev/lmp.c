#include <stdio.h>
#include <string.h>
#include "dev/leds.h"
#include "uspi.h"
#include "contiki-conf.h"
#include <gpio.h>
#include <stm32f10x_nvic.h>

#include "lmp.h"

static inline void spi_lmp_cs_high()
{
  GPIOB->BSRR = 1<<12;
}

static inline void spi_lmp_cs_low()
{
  GPIOB->BRR = 1<<12;
}

//init
void lmp_init(void)
{
  int i;
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

  spi_lmp_cs_high();

  printf("lmp_init done - status: %x\n\r", lmp_get_status());
}

//	Checks registers DATA_ONLY_1 & DATA_ONLY_2...
uint32_t lmp_get_status(void)
{
   return lmp_read_data(0x09, 0x02);
}

//	Setup lmp for 3-wire PT100
void lmp_setup(){
	uint8_t tmp = 0;
	
	//RESETCN = 0xC3 => register & conversion reset
	lmp_write_data(RESETCN,0xC3);	
	
	//CH0_INPUTCN = 0x41 => VINP = VIN0, VINN = VIN1, VREFx2
	lmp_write_data(CH0_INPUTCN,0x41);
	//CH0_CONFIG = 0x0B => ODR=1.6775PS, Gain=32, FGA=on, BUF=included
	lmp_write_data(CH0_CONFIG,0x0B);
	//SCALCN = 0x00 => System Calibration Mode = Normal
	lmp_write_data(SCALCN,0x00);
	//BGCALCN = 0x02 => BackgroundCalibrationMode = 2, Offset & Gain correction
	lmp_write_data(BGCALCN,0x02);
	//ADC_AUXCN = 0x02 => IB=200uA, Ext.Clk detection
	lmp_write_data(ADC_AUXCN,0x02);
	//CH_SCAN = 0x00 => Single Channel continius conversion, CH0
		//CH_STS must bee checked before writing to CH_SCAN
		tmp = lmp_read_data(CH_STS,1);
		//printf("tmp: %x",tmp);
		tmp = (tmp |= 0xFD);
	//	printf(", %x\r\n",tmp);
		if(tmp > 0xFD){
			printf("CH_SCAN not ready, skipping write\r\n");
		}else{
		 	lmp_write_data(CH_SCAN,0x00);
		}
		while(lmp_read_data(ADC_AUXCN,1)!= 0x02){
			lmp_write_data(ADC_AUXCN,0x02);
		}printf("ADC_AUXCN: %x\r\n",lmp_read_data(ADC_AUXCN,1));
}

//	read register
uint32_t lmp_read_data(uint8_t adr, uint8_t nr)
{
	uint32_t data = 0;
	uint8_t tmp,tmph,tmpl;
	tmp=0;
	tmph=0;
	tmpl=0;

	//convert number of bytes to recive
	tmp = nr;
	switch (nr){
	case 2:  nr = 2;
	break;
	case 3:  nr = 4;
	break;
	default: nr = 0;
	break;
	}

	tmph |= ((0xF0 & adr) >> 1*4);
	tmpl |= ((nr |= 0x8) << 1*4);
	tmpl |= ((0x0F & adr) << 0*4);
	//printf("tmph: %x tmpl: %x adr: %x nr: %x\r\n",tmph,tmpl,adr,nr);

	spi_lmp_cs_low();
	  (void)uspi_txrx(SPI_BUS_2, 0x10);
	  (void)uspi_txrx(SPI_BUS_2,tmph);
	  (void)uspi_txrx(SPI_BUS_2,tmpl);

	//Builds Word
	switch (tmp){
	case 3: 
	  tmp = uspi_txrx(SPI_BUS_2, 0x00);
	  data |= (tmp << 2*8);
	case 2: 
	  tmp = uspi_txrx(SPI_BUS_2, 0x00);
	  data |= (tmp << 1*8);
	default: 
	  tmp = uspi_txrx(SPI_BUS_2, 0x00);
	  data |= (tmp << 0*8);
	break;
	}

	/*tmp = uspi_txrx(SPI_BUS_2, 0x00);
	if((tmp != 0)){
		printf("something is worng, more data exists..%x\r\n",tmp);
	}*/

	spi_lmp_cs_high();

return data;
}

//	Write register
void lmp_write_data(uint8_t adr, uint8_t data)
{
	uint8_t tmph,tmpl;
	tmph=0;
	tmpl=0;

	tmph |= ((0xF0 & adr) >> 1*4);
	tmpl |= ((0x0F & adr) << 0*4);
	//printf("adr: %x tmph: %x tmpl: %x data: %x\r\n",adr,tmph,tmpl,data);

	spi_lmp_cs_low();
  	(void)uspi_txrx(SPI_BUS_2, 0x10);
	(void)uspi_txrx(SPI_BUS_2,tmph);
	(void)uspi_txrx(SPI_BUS_2,tmpl);
	(void)uspi_txrx(SPI_BUS_2, data);
	
	spi_lmp_cs_high();

}


