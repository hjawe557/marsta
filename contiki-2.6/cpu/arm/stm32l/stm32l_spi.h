#ifndef STM32L_SPI_H
#define STM32L_SPI_H

typedef struct {
        volatile uint16_t CR1;
        uint16_t R0;
        volatile uint16_t CR2;
        uint16_t R1;
        volatile uint16_t SR;
        uint16_t R2;
        volatile uint16_t DR;
        uint16_t R3;
        volatile uint16_t CRCPR;
        uint16_t R4;
        volatile uint16_t RXCRCR;
        uint16_t R5;
        volatile uint16_t TXCRCR;
        uint16_t R6;  
} SPI_;

#define SPI1_BASE                       (APB2PERIPH_BASE + 0x3000)
#define SPI2_BASE                       (APB1PERIPH_BASE + 0x3800)

#define SPI1                            ((SPI_ *)SPI1_BASE)
#define SPI2                            ((SPI_ *)SPI2_BASE)

#define  SPI_CR1_CPHA                   ((uint16_t)0x0001)
#define  SPI_CR1_CPOL                   ((uint16_t)0x0002)
#define  SPI_CR1_MSTR                   ((uint16_t)0x0004)

#define  SPI_CR1_BR                     ((uint16_t)0x0038)
#define  SPI_CR1_BR_0                   ((uint16_t)0x0008)
#define  SPI_CR1_BR_1                   ((uint16_t)0x0010)
#define  SPI_CR1_BR_2                   ((uint16_t)0x0020)

#define  SPI_CR1_SPE                    ((uint16_t)0x0040)
#define  SPI_CR1_LSBFIRST               ((uint16_t)0x0080)
#define  SPI_CR1_SSI                    ((uint16_t)0x0100)
#define  SPI_CR1_SSM                    ((uint16_t)0x0200)
#define  SPI_CR1_RXONLY                 ((uint16_t)0x0400)
#define  SPI_CR1_DFF                    ((uint16_t)0x0800)
#define  SPI_CR1_CRCNEXT                ((uint16_t)0x1000)
#define  SPI_CR1_CRCEN                  ((uint16_t)0x2000)
#define  SPI_CR1_BIDIOE                 ((uint16_t)0x4000)
#define  SPI_CR1_BIDIMODE               ((uint16_t)0x8000)

#define  SPI_CR2_RXDMAEN                ((uint8_t)0x01)
#define  SPI_CR2_TXDMAEN                ((uint8_t)0x02)
#define  SPI_CR2_SSOE                   ((uint8_t)0x04)
#define  SPI_CR2_ERRIE                  ((uint8_t)0x20)
#define  SPI_CR2_RXNEIE                 ((uint8_t)0x40)
#define  SPI_CR2_TXEIE                  ((uint8_t)0x80)

#define  SPI_SR_RXNE                    ((uint8_t)0x01)
#define  SPI_SR_TXE                     ((uint8_t)0x02)
#define  SPI_SR_CRCERR                  ((uint8_t)0x10)
#define  SPI_SR_MODF                    ((uint8_t)0x20)
#define  SPI_SR_OVR                     ((uint8_t)0x40)
#define  SPI_SR_BSY                     ((uint8_t)0x80)

#define  SPI_CRCPR_CRCPOLY              ((uint16_t)0xFFFF)  

#define  SPI_RXCRCR_RXCRC               ((uint16_t)0xFFFF)  

#define  SPI_TXCRCR_TXCRC               ((uint16_t)0xFFFF)  

#endif
