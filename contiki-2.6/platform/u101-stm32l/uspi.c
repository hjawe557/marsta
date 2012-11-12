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
#include <stm32l.h>
#include <stm32l_spi.h>
#include <stm32l_rcc.h>
#include <stdlib.h>
#include <uspi.h>

/**
 * \brief   Set up SPI controller before use
 * \bus     Selected SPI bus (SPI1, SPI2, etc)
 *
 * Initiate SPI bus before use. Take care to check
 * if you need to do modifications to things like
 * clock polarity, phase and bit ordering
 *
 */

void SPI1_handler(void) __attribute__((interrupt));

void SPI1_handler(void)
{
  return;
}

uint32_t spi_buses[] = { SPI1_BASE, SPI2_BASE, NULL };

void uspi_init(uint32_t busnumber, uint8_t rate)
{
  SPI_ *sb = (SPI_ *)spi_buses[busnumber];
  uint32_t tmp;

  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);
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
    (0x0008 | SPI_CR1_BR);

  sb->CR1 = tmp;

  /* Enabled */
  sb->CR1 |= SPI_CR1_SPE;
}

void uspi_tx(uint32_t busnumber, uint16_t data)
{
  SPI_ *sb = (SPI_ *)spi_buses[busnumber];

  sb->DR = data;
}

void uspi_send(uint32_t busnumber, uint16_t data)
{
  SPI_ *sb = (SPI_ *)spi_buses[busnumber];

  sb->DR = data;
  
  /* Wait until done */
  while (sb->SR & SPI_SR_BSY)
    ;
}

uint16_t uspi_txrx(uint32_t busnumber, uint16_t data)
{
  SPI_ *sb = (SPI_ *)spi_buses[busnumber];

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
  SPI_ *sb = (SPI_ *)spi_buses[busnumber];

  return sb->DR;
}



