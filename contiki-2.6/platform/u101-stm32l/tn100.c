#include <stdint.h>
#include <stdio.h>
#include <uspi.h>
#include <tn100.h>

#if 0
static inline void tn100_cs_high()
{
  GPIOA->BSRR = 1<<4;
}

static inline void tn100_cs_low()
{
  GPIOA->BRR = 1<<4;
}
#endif

uspi_cs_fn tn100_spi_cs;
uint32_t tn100_spi_bus;
tn100_reset_fn tn100_reset;

uint8_t tn100_read_reg(uint8_t a)
{
  uint8_t val;
  uint8_t tmp;

  tn100_spi_cs(SPI_CS_LOW);

  /* Read 1 byte */
  (void)uspi_txrx(tn100_spi_bus, TN100_SPI_RX | TN100_SPI_LEN(0x01));
  /* Select address */
  tmp = uspi_txrx(tn100_spi_bus, a);
  /* Read */
  val = uspi_txrx(tn100_spi_bus, 0x00);

  tn100_spi_cs(SPI_CS_HIGH);

  printf("TN100 [0x%02x]: 0x%02x 0x%02x\r\n", 
         a, tmp, val);

  return val;
}

void tn100_write_reg(uint8_t a, uint8_t val)
{

  tn100_spi_cs(SPI_CS_LOW);

  /* Write 1 byte */
  (void)uspi_txrx(tn100_spi_bus, 0x81);
  /* Select address */
  (void)uspi_txrx(tn100_spi_bus, a);
  /* Write */
  (void)uspi_txrx(tn100_spi_bus, val);

  tn100_spi_cs(SPI_CS_HIGH);

  return;
}

void stupiddelay(uint32_t n) {
  int i;
  int volatile dummy;
  for(i=0; i<n; i++) {
    dummy = i;
  }
}

void tn100_init(uint32_t spi_bus, uspi_cs_fn spi_cs, tn100_reset_fn reset)
{
  int i;

  tn100_reset = reset;
  tn100_spi_bus = spi_bus;
  tn100_spi_cs= spi_cs;

  /* Configure I/O */
#if 0
  /* CS */
  GPIO_CONF_OUTPUT_PORT(A, 4, PUSH_PULL, 50);
  /* CLOCK */
  GPIO_CONF_OUTPUT_PORT(A, 5, ALT_PUSH_PULL, 50);
  /* MISO */
  GPIO_CONF_OUTPUT_PORT(A, 6, ALT_PUSH_PULL, 50);
  /* MOSI */
  GPIO_CONF_OUTPUT_PORT(A, 7, ALT_PUSH_PULL, 50);

  GPIO_CONF_INPUT_PORT(A, 11, FLOATING);
  /* Reset */
  GPIO_CONF_OUTPUT_PORT(A, 1, PUSH_PULL, 50);
#endif


  uspi_init(tn100_spi_bus, 000);
  tn100_spi_cs(SPI_CS_HIGH);
  tn100_reset(TN100_RESET_INACTIVE);
  stupiddelay(10000);
  tn100_reset(TN100_RESET_ACTIVE);
  stupiddelay(10000);
  tn100_reset(TN100_RESET_INACTIVE);

#if 0
  /* Reset high */
  GPIOA->BSRR = 1<<1;
  printf("Reset high\r\n");
  stupiddelay(10000);

  /* Reset low */
  GPIOA->BRR = 1<<1;
  printf("Reset low\r\n");
  stupiddelay(10000);

  /* Reset high */
  GPIOA->BSRR = 1<<1;
  printf("Reset high\r\n");
  stupiddelay(10000);
#endif

  for (i=0; i<0x20; i++) {
    (void)tn100_read_reg(i);
    stupiddelay(10000);
  }

}
