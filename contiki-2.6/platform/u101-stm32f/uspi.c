/*
 * Copyright (c) 2011, Erik Jansson (erik at upwis.com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name of the <organization> nor the names of its
 *       contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <stm32f10x_map.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <uspi.h>

void SPI1_handler(void) __attribute__((interrupt));

void SPI1_handler(void)
{
  return;
}

/*
Bits 13:11 PPRE2[2:0]: APB high-speed prescaler (APB2)

Set and cleared by software to control the division factor of the APB
High speed clock (PCLK2).

0xx: HCLK not divided
100: HCLK divided by 2
101: HCLK divided by 4
110: HCLK divided by 8
111: HCLK divided by 16
*/
static uint16_t uspi_apb_div(uint16_t pre)
{
  if (pre > 7) {
    printf("Bad APB prescaler: 0x%04x\n", pre);
    return 0xffff;
  }

  if (pre < 4) {
    return 1;
  }

  switch(pre & 0x0007) {
  case 0x0004:
    return 2;
    break;
  case 0x0005:
    return 4;
    break;
  case 0x0006:
    return 8;
    break;
  case 0x0007:
    return 16;
    break;
  default:
    printf("Unknown APB prescaler: 0x%04x\n", pre);
    return 1;
  }
}

uint16_t uspi_pow2(uint16_t exp)
{
    uint16_t result = 1;
    uint16_t base = 2;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

uint32_t spi_buses[] = { SPI1_BASE, SPI2_BASE, 0 };

void uspi_init(uint32_t busnumber, uint8_t clkdiv)
{
  SPI_TypeDef *sb = (SPI_TypeDef *)spi_buses[busnumber];
  uint32_t tmp;
  uint32_t pclkdiv;
  uint32_t pclk;
  uint32_t actual_clkdiv;
  /*
    With pclock=1, clkdiv give these SPI clocks:
    clkdiv            SPI clk,     SPI clk,
                      MCLK=32 MHz  MCLK=48Mhz
    000: fPCLK/2      16000000     24000000  
    001: fPCLK/4      8000000      12000000  
    010: fPCLK/8      4000000      6000000   
    011: fPCLK/16     2000000      3000000   
    100: fPCLK/32     1000000      1500000   
    101: fPCLK/64     500000       750000    
    110: fPCLK/128    250000       375000    
    111: fPCLK/256    125000       187500    
  */
  pclkdiv = uspi_apb_div((RCC->CFGR & RCC_CFGR_PPRE2) >> 11);
  pclk = (int)(MCK / pclkdiv);
  actual_clkdiv = (uspi_pow2(clkdiv) << 1);
  printf("uspi: Peripheral clock runs at %u MHz (APB divisor is %u)\n", 
         (unsigned int)pclk, (unsigned int)pclkdiv);
  printf("uspi: Using clkdiv %u, the resulting SPI clock rate is %u Hz (writing: 0x%02x)\n", 
         (unsigned int)(actual_clkdiv), 
         (unsigned int)(pclk / actual_clkdiv), 
         (unsigned int)(clkdiv));

  RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN |
                   RCC_APB2ENR_IOPAEN |
                   RCC_APB2ENR_IOPBEN |
                   RCC_APB2ENR_SPI1EN);
  RCC->APB1ENR |= (RCC_APB1ENR_SPI2EN);

  /* Should disable spi before changing stuff */
  
  tmp =
    /* Master mode */
    SPI_CR1_MSTR |
    /*
    SPI_CR1_CPOL |
    SPI_CR1_CPHA |
    */
    SPI_CR1_SSM |
    SPI_CR1_SSI |
    /* See table above */
    ((clkdiv << 3) & SPI_CR1_BR);
  printf("uspi: CR1: %04x\n", tmp);
  sb->CR1 = tmp;

  /* Enabled */
  sb->CR1 |= SPI_CR1_SPE;
}

void uspi_tx(uint32_t busnumber, uint16_t data)
{
  SPI_TypeDef *sb = (SPI_TypeDef *)spi_buses[busnumber];

  sb->DR = data;
}

void uspi_send(uint32_t busnumber, uint16_t data)
{
  SPI_TypeDef *sb = (SPI_TypeDef *)spi_buses[busnumber];

  sb->DR = data;
  
  /* Wait until done */
  while (sb->SR & SPI_SR_BSY)
    ;
}

uint16_t uspi_txrx(uint32_t busnumber, uint16_t data)
{
  SPI_TypeDef *sb = (SPI_TypeDef *)spi_buses[busnumber];

  while(!(sb->SR & SPI_SR_TXE))
    ;

  sb->DR = data;

  while(!(sb->SR & SPI_SR_RXNE))
    ;

  return sb->DR;
}

#ifdef SPI_T
char spi_test_ptn[] = { 0x55, 0xaa, 0x65, 0x72, 
			0x69, 0x6B, 0x55, 0xaa };

#endif

uint16_t uspi_read(uint32_t busnumber)
{
  SPI_TypeDef *sb = (SPI_TypeDef *)spi_buses[busnumber];

  return sb->DR;
}



