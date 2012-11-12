#ifndef TN100_H
#define TN100_H

#include <stdint.h>
#include <uspi.h>

#define TN100_SPI_RX 0x00
#define TN100_SPI_TX 0x80
#define TN100_SPI_LEN(x) (0x7f & x)

#define TN100_RESET_ACTIVE   1
#define TN100_RESET_INACTIVE 0
typedef void (*tn100_reset_fn)(uint8_t active);

void tn100_init(uint32_t spi_bus, uspi_cs_fn spi_cs, tn100_reset_fn reset);

#endif
