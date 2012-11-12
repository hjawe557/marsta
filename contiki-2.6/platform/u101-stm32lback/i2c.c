#include <stdint.h>
#include <stdlib.h>
#include <gpio.h>
#include <stm32l_i2c.h>
#include <stm32l_rcc.h>

#include <i2c.h>

uint32_t i2c_buses[] = { I2C1_BASE, I2C2_BASE, NULL };


/*
  This is painful.  I2C operations implemented according to app. note
  AN2824 - "STM32F10xxx I2C optimized examples" using polling read and
  write in order to test the on-board I2C peripherals.  Ideally these
  should be implemented using interrupts and/or DMA.x */

void i2c_init(const uint32_t busnum, uint32_t rate)
{
  I2C_ *ib = (I2C_ *)i2c_buses[busnum];

  /* Ensure that the external bus doesn't interfere with i2c */
  /* AFIO->MAPR2 |= 0x400; */

  /*
    Enable alternate functions, port B and i2c
   */
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

  if (I2C_BUS_1 == busnum) {
    gpio_conf_af(GPIOB, 6, GPIO_OUTPUT_TYPE_PPULL, GPIO_OSPEED_40MHZ, GPIO_RESISTORS_PULLUP);
    gpio_conf_af(GPIOB, 7, GPIO_OUTPUT_TYPE_PPULL, GPIO_OSPEED_40MHZ, GPIO_RESISTORS_PULLUP);
    gpio_map_af(GPIOB, 6, GPIO_AF_I2C1);
    gpio_map_af(GPIOB, 7, GPIO_AF_I2C1);
  } else {
    /* We're careful with this for now. It's shared with the debug USART */
    return;
  }
#if 0
  /* SCL1 */
  GPIO_CONF_OUTPUT_PORT(B, 6, ALT_OPEN_DRAIN, 50);
  /* SDA1 */
  GPIO_CONF_OUTPUT_PORT(B, 7, ALT_OPEN_DRAIN, 50);
#endif

#if 0  
  /* Disable */
  ib->CR1 &= ~(I2C_CR1_PE);
  /* Fast */
  /* ib->CCR &= I2C_CCR_FS; */
  /* CCR */
  tmp = ib->CCR;
  tmp &= 0xf000;
  tmp |= 0x5e;
  ib->CCR = tmp;
  /* Freq */
  tmp = ib->CR2;
  tmp &= 0xffc0;
  tmp |= 0x20;
  ib->CR2 = tmp;
  /* TRISE */
  ib->TRISE = 0x0b;
#else
  /* Disable peripheral while configuring */
  ib->CR1 &= ~(I2C_CR1_PE);

  /* Bit 14 should be kept at 1 by software */
  ib->OAR1 = 0x4000;
  /* Frequency, 8 MHz  */
  ib->CR2 = 0x0008; 
  /* clock configure, period is 0x28 (dec 40) * PCLK */
  ib->CCR = 0x0028;
  /* Rise time... */
  ib->TRISE = 0x0009;
  /* Set ACK bit */
  ib->CR1 |= I2C_CR1_ACK;
#endif
  /* Enable */
  ib->CR1 |= I2C_CR1_PE;
}

#if 0
static void i2c_send_address(uint32_t bus, uint8_t slave, uint8_t rw)
{
  I2C_ *ib = (I2C_ *)bus;
  ib->DR = (uint8_t)((slave) | rw);
}
#endif

static void i2c_busywait(uint32_t n) {
	for(; n != 0; n--);
}

static int i2c_start(uint32_t bus) {

	I2C_ *ib = (I2C_ *)bus;
        int i;
        int timeout;

        timeout = 0x10000;

      	/* Send start */
	ib->CR1 |= I2C_CR1_START;
	
	/* Wait until start is sent and bus is switched to master */
	for (i=0; 
	     i < timeout && !((ib->SR1 & I2C_SR1_SB) &&
			      (ib->SR2 & (I2C_SR2_MSL | I2C_SR2_BUSY))); 
	     i++) {
		i2c_busywait(100);
	}

        if (i >= timeout) {
		return -2;
	}

        return 0;
}

int i2c_read(uint32_t bus, uint8_t address, const uint8_t len, uint8_t *buf) {

 	I2C_ *ib = (I2C_ *)bus;
	int timeout;
	int i;
	int n;
       	volatile uint32_t tmp;

        if (i2c_start(bus)) {
                return -1;
        }

	/* Send address, 1 for read */
	ib->DR = (uint8_t)(address | 0x01);
        
        timeout = 0x10000;
	
        /* Wait until address is set */
        for (i=0; 
             i < timeout && !(ib->SR1 & I2C_SR1_ADDR); 
             i++) {
                i2c_busywait(100);
        }

        if (i >= timeout) {
                //return -3;
        }
	
        if (len == 1) {
                
                /* ACK = 0 */
                ib->CR1 &= ~(I2C_CR1_ACK);
		
                /* Disable interrupts */
                ib->CR2 &= ~(I2C_CR2_ITBUFEN | 
                             I2C_CR2_ITEVTEN | 
                             I2C_CR2_ITERREN);
                
                /* Read SR1 and SR2 to clear the ADDR bit in SR1 */
                tmp = ib->SR1;
                tmp = ib->SR2;
		
                /* Set STOP */
                ib->CR1 |= I2C_CR1_STOP;
		
                /* Enable interrupts */
                ib->CR2 |= (I2C_CR2_ITBUFEN | 
                            I2C_CR2_ITEVTEN | 
                            I2C_CR2_ITERREN);
                
                /* Wait until RXNE == 1 */
                for (i=0; 
                     i < timeout && !(ib->SR1 & I2C_SR1_RXNE); 
                     i++) {
                        i2c_busywait(100);
                }
		
                if (i >= timeout) {
                        return -4;
                }
		
                /* Read data */
                tmp = (ib->DR);
		
                /* len = 1 so we only want one byte */
                buf[0] = (uint8_t)tmp;
		
                /* Wait until STOP is cleared by hw */
                for (i=0; 
                     i < timeout && (ib->CR1 & I2C_CR1_STOP); 
                     i++) {
                        i2c_busywait(100);
                }
		
                if (i >= timeout) {
                        return -5;
                }
		
                /* ACK = 1 so we're ready for next reception */
                ib->CR1 |= (I2C_CR1_ACK);
		
        } else if (len == 2) {
                
                /* POS = 1 */
                ib->CR1 |= (I2C_CR1_POS);
		
                /* Disable interrupts */
                ib->CR2 &= ~(I2C_CR2_ITBUFEN | 
                             I2C_CR2_ITEVTEN | 
                             I2C_CR2_ITERREN);
                
                /* Clear ADDR in SR1 by reading SR1 and SR2 */
                tmp = ib->SR1;
                tmp = ib->SR2;
		
                /* ACK = 0 */
                ib->CR1 &= ~(I2C_CR1_ACK);
		
                /* Enable interrupts */
                ib->CR2 |= (I2C_CR2_ITBUFEN | 
                            I2C_CR2_ITEVTEN | 
                            I2C_CR2_ITERREN);
                
                /* Wait for BTF == 1 */
                for (i=0; 
                     i < timeout && !(ib->SR1 & I2C_SR1_BTF); 
                     i++) {
                        i2c_busywait(100);
                }
		
                if (i >= timeout) {
                        return -6;
                }
		
                /* Disable interrupts */
                ib->CR2 &= ~(I2C_CR2_ITBUFEN | 
                             I2C_CR2_ITEVTEN | 
                             I2C_CR2_ITERREN);
                
		
                /* STOP = 1 */
                ib->CR1 |= I2C_CR1_STOP;
		
                /* Wait until RXNE == 1 */
                for (i=0; 
                     i < timeout && !(ib->SR1 & I2C_SR1_RXNE); 
                     i++) {
                        i2c_busywait(100);
                }
		
                /* Read data 1 (0) */
                tmp = (ib->DR);
		
                /* len = 1 so we only want one byte */
                buf[0] = (uint8_t)tmp;
		
                /* Enable interrupts */
                ib->CR2 |= (I2C_CR2_ITBUFEN | 
                            I2C_CR2_ITEVTEN | 
                            I2C_CR2_ITERREN);
                
                /* Read data 2 (1) */
                tmp = (ib->DR);
		
                /* len = 1 so we only want one byte */
                buf[1] = (uint8_t)tmp;
		
                /* Wait until STOP is cleared by hw */
                for (i=0; 
                     i < timeout && (ib->CR1 & I2C_CR1_STOP); 
                     i++) {
                        i2c_busywait(100);
                }
		
                if (i >= timeout) {
                        return -7;
                }
		
                /* POS = 0 */
                ib->CR1 &= ~(I2C_CR1_POS);
		
                /* ACK = 1 so we're ready for next reception */
                ib->CR1 |= (I2C_CR1_ACK);
		
        } else {
                /* We have read 0 bytes yet */
                n = 0;
                
                /* Read all bytes but the last in this loop */
                while (n+3 >= len) {
                        
                        /* Wait until RXNE == 1 */
                        for (i=0; 
                             i < timeout && !(ib->SR1 & I2C_SR1_RXNE); 
                             i++) {
                                i2c_busywait(100);
                        }
			
                        if (i >= timeout) {
                                return -11;
                        }
                        
                        /* Read data N */
                        tmp = (ib->DR);
			
                        /* len = 1 so we only want one byte */
                        buf[n] = (uint8_t)tmp;
                        
                        /* We have read one more byte */
                        n++;
                }
                
                /* Wait until BTF == 1 is set */
                for (i=0; 
                     i < timeout && !(ib->SR1 & I2C_SR1_BTF); 
                     i++) {
                        i2c_busywait(100);
                }
		
                if (i >= timeout) {
                        /* Maybe we should do something better here? */
                        return -83;
                }
		
                /* ACK = 0 */
                ib->CR1 &= ~(I2C_CR1_ACK);
                
                /* Disable interrupts */
                ib->CR2 &= ~(I2C_CR2_ITBUFEN | 
                             I2C_CR2_ITEVTEN | 
                             I2C_CR2_ITERREN);
                
                /* STOP = 1 */
                ib->CR1 |= I2C_CR1_STOP;
		
                /* Read data n */
                tmp = (ib->DR);
		
                /* len = 1 so we only want one byte */
                buf[n] = (uint8_t)tmp;
                
                /* We have read one more byte */
                n++;
		
                /* Enable interrupts */
                ib->CR2 |= (I2C_CR2_ITBUFEN | 
                            I2C_CR2_ITEVTEN | 
                            I2C_CR2_ITERREN);
                
                /* Wait until RXNE == 1 */
                for (i=0; 
                     i < timeout && !(ib->SR1 & I2C_SR1_RXNE); 
                     i++) {
                        i2c_busywait(100);
                }
		
                if (i >= timeout) {
                        return -11;
                }
		
                /* Read data N */
                tmp = (ib->DR);
		
                /* len = 1 so we only want one byte */
                buf[n] = (uint8_t)tmp;
		
                /* We have read one more byte */
                n++;
                
                /* Wait until STOP is cleared by hw */
                for (i=0; 
                     i < timeout && (ib->CR1 & I2C_CR1_STOP); 
                     i++) {
                        i2c_busywait(100);
                }
		
                if (i >= timeout) {
                        return -7;
                }
		
                /* ACK = 1 so we're ready for next reception */
                ib->CR1 |= (I2C_CR1_ACK);
                
                return -20;
        }
        return 0;

}

#define I2C_WAIT(x, t, e) \
        { int _i; \
          for (_i=0; _i < t && x; _i++) \
              { i2c_busywait(100); } \
          if (_i >= timeout) return e; \
        }

int i2c_write(uint32_t bus, uint8_t address, const uint8_t len, uint8_t *buf,
                       uint8_t nostop) {

	I2C_ *ib = (I2C_ *)bus;
	int timeout;
	int i, j;
	volatile uint32_t tmp;

        timeout = 0x10000;
        
        /* LSB must be zero, since it differentiates between read and write */
        if (address & 0x01) {
                return -1;
        }

        if (i2c_start(bus)) {
                return -2;
        }

	/* Send address, low bit 0 for read */
	ib->DR = (uint8_t)address;

        /* Wait until address is set */
        I2C_WAIT(!(ib->SR1 & I2C_SR1_ADDR), timeout, -3);
#if 0
        for (i=0; i < timeout && !(ib->SR1 & I2C_SR1_ADDR); i++) {
                i2c_busywait(100);
        }
		
        if (i >= timeout) {
                return -81;
        }
#endif
        /* Clear ADDR in SR1 by reading SR1 and SR2 */
        tmp = ib->SR1;
        tmp = ib->SR2;

        for (j=0;
             j<len;
             j++) {
                /* Send byte */
                ib->DR = buf[j];

                /* Wait until byte is sent is set */
                for (i=0; 
                     i < timeout && !(ib->SR1 & I2C_SR1_TXE); 
                     i++) {
                        i2c_busywait(100);
                }
			
                if (i >= timeout) {
                        /* Maybe we should do something better here? */
                        return -82;
                }
			
        }

        /* Wait until BTF == 1 is set */
        for (i=0; 
             i < timeout && !(ib->SR1 & I2C_SR1_BTF); 
             i++) {
                i2c_busywait(100);
        }
		
        if (i >= timeout) {
                /* Maybe we should do something better here? */
                return -83;
        }

        if (!nostop) {
                ib->CR1 |= (I2C_CR1_STOP);
			
                /* Wait until STOP is cleared by hw */
                for (i=0; 
                     i < timeout && (ib->CR1 & I2C_CR1_STOP); 
                     i++) {
                        i2c_busywait(100);
                }
			
                if (i >= timeout) {
                        return -84;
                }
        }
        
        return 0;
}


int i2c_transact(uint32_t bus, uint8_t address, 
                          uint8_t rw, const uint8_t len,
                          uint8_t nostop,
                          uint8_t *buf)
{
	if (len == 0) {
		return -1;
	}
	
	if (1 == rw) {
                return i2c_read(bus, address, len, buf);
	} else {
                return i2c_write(bus, address, len, buf, nostop);
	}
}

