#include <gpio.h>
#include <spi.h>

static inline void spi_flash_cs_high()
{
  GPIOB->BSRR = 1<<2;
}

static inline void spi_flash_cs_low()
{
  GPIOB->BRR = 1<<2;
}

void spi_flash_init(void)
{
  /* CS */
  GPIO_CONF_OUTPUT_PORT(B, 2, PUSH_PULL, 50);

  /* CLOCK */
  GPIO_CONF_OUTPUT_PORT(A, 5, ALT_PUSH_PULL, 50);
  /* MISO */
  GPIO_CONF_OUTPUT_PORT(A, 6, ALT_PUSH_PULL, 50);
  /*  GPIO_CONF_INPUT_PORT(A, 6, FLOATING); */
  /* MOSI */
  GPIO_CONF_OUTPUT_PORT(A, 7, ALT_PUSH_PULL, 50);

  spi_flash_cs_high();

  spi_init(SPI1_BASE, 000);
}



uint32_t spi_flash_readid(void)
{
  uint32_t id;
  uint32_t a,b,c;

  spi_flash_cs_low();

  (void)spi_txrx(SPI1_BASE, 0x9F);
  a = spi_txrx(SPI1_BASE, 0x0);
  b = spi_txrx(SPI1_BASE, 0x0);
  c = spi_txrx(SPI1_BASE, 0x0);

  spi_flash_cs_high();

  id = (a << 16) | (b << 8) | c;

  return id;
}

