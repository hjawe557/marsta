#include <debug-uart.h>
#include <string.h>
#include <stm32f10x_map.h>
#include <stm32f10x_dma.h>
#include <gpio.h>
#include <nvic.h>
#include "leds.h"
#include "contiki-conf.h"

#ifdef DBG_ON_USART1
#define DBG_UART USART1
#define DBG_DMA_NO 1
#define DBG_DMA_CHANNEL_NO 4
#define STM32_DBG_TXPORT A
#define STM32_DBG_TXPIN 9
#define STM32_DBG_RXPORT A
#define STM32_DBG_RXPIN 10
#elif DBG_ON_USART3
#define DBG_UART USART3
#define DBG_DMA_NO 1
#define DBG_DMA_CHANNEL_NO 2
#define STM32_DBG_TXPORT B
#define STM32_DBG_TXPIN 10
#define STM32_DBG_RXPORT B
#define STM32_DBG_RXPIN 11
#endif

#ifndef DBG_UART
#define DBG_UART USART1
#endif

/* All USART TX DMA is on DMA1 */
#ifndef DBG_DMA_NO
#define DBG_DMA_NO 1
#endif

/* See DMA request mapping in datasheet 
   For USART1 TX, use DMA1, ch 4
   For USART2 TX, use DMA1, ch 7
   For USART3 TX, use DMA1, ch 2
 */
#ifndef DBG_DMA_CHANNEL_NO
#define DBG_DMA_CHANNEL_NO 4
#endif

#define _DBG_DMA_NAME(x) DMA##x
#define DBG_DMA_NAME(x) _DBG_DMA_NAME(x)
#define DBG_DMA DBG_DMA_NAME(DBG_DMA_NO)

#define _DMA_CHANNEL_NAME(x,c) DMA ## x ## _Channel ## c
#define DMA_CHANNEL_NAME(x,c) _DMA_CHANNEL_NAME(x,c)
#define DBG_DMA_CHANNEL  DMA_CHANNEL_NAME(DBG_DMA_NO, DBG_DMA_CHANNEL_NO)

#define _DBG_DMA_CHANNEL_IFCR_CGIF(c) DMA_IFCR_CGIF ## c
#define _XDBG_DMA_CHANNEL_IFCR_CGIF(c) _DBG_DMA_CHANNEL_IFCR_CGIF(c)
#define DBG_DMA_CHANNEL_IFCR_CGIF \
_XDBG_DMA_CHANNEL_IFCR_CGIF(DBG_DMA_CHANNEL_NO)

#ifndef DBG_XMIT_BUFFER_LEN
#define DBG_XMIT_BUFFER_LEN 2048
#endif

static unsigned char xmit_buffer[DBG_XMIT_BUFFER_LEN];
#define XMIT_BUFFER_END &xmit_buffer[DBG_XMIT_BUFFER_LEN]

void
dbg_setup_uart_default()
{
  /* TODO: Check which ports really need to be enabled for USART 1, 2 and 3 */
  RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN |
		   RCC_APB2ENR_IOPAEN |
		   RCC_APB2ENR_IOPBEN);
#ifdef DBG_ON_USART1
  RCC->APB2ENR |= (RCC_APB2ENR_USART1EN);
#elif DBG_ON_USART3
  RCC->APB1ENR |= (RCC_APB1ENR_USART3EN);
#endif
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;

  /* TODO: This just covers remapping of USART1. What about 2 and 3? */
  if (STM32_DBG_REMAP)
    AFIO_REMAP(AFIO_MAPR_USART1_REMAP, AFIO_MAPR_USART1_REMAP);

  /* The ## operator doesn't do expansion so we can't pass a 
     defined variable directly to the GPIO_CONF_*_PORT macros. */
#define DBG_UART_TX_PORT(port,pin,pp,n)		\
  GPIO_CONF_OUTPUT_PORT(port,pin,pp,n)
#define DBG_UART_RX_PORT(port,pin,f)		\
  GPIO_CONF_INPUT_PORT(port,pin,f)

  DBG_UART_TX_PORT(STM32_DBG_TXPORT, STM32_DBG_TXPIN, ALT_PUSH_PULL, 50);
  DBG_UART_RX_PORT(STM32_DBG_RXPORT, STM32_DBG_RXPIN, FLOATING);

#undef DBG_UART_TX_PORT
#undef DBG_UART_RX_PORT

  DBG_UART->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;  
  DBG_UART->CR2 = 0;
  DBG_UART->CR3 = USART_CR3_DMAT;

  /* USART1 works on PCLK2, all other on PCLK1, so BRR needs to be different */
#ifdef DBG_ON_USART1
  DBG_UART->BRR= 0x1a1;
#else
  /* PCLK1 is 24 MHz, USARTDIV = fPCK / (16 * 115200) = 13.02 -> BRR = 0xd1 */
  DBG_UART->BRR= 0x0d1;
#endif
}

/* Valid data in head to tail-1 */
/* Read position */
static unsigned char * volatile xmit_buffer_head = xmit_buffer;

/* Write position */
static unsigned char * volatile xmit_buffer_tail = xmit_buffer;

/* xmit_buffer_head == xmit_buffer_tail means empty so we can only store
   DBG_XMIT_BUFFER_LEN-1 characters */

volatile unsigned char dma_running = 0;
static unsigned char * volatile dma_end;
void DMA1_Channel4_handler() __attribute__((interrupt));
void DMA2_Channel4_5_handler() __attribute__((interrupt));


static void
update_dma(void)
{
  if (xmit_buffer_tail == xmit_buffer_head) return;
  DBG_DMA_CHANNEL->CCR = (DMA_Priority_Low |
			  DMA_PeripheralDataSize_Byte |
			  DMA_MemoryDataSize_Byte |
			  DMA_PeripheralInc_Disable |
			  DMA_MemoryInc_Enable |
			  DMA_Mode_Normal |
			  DMA_DIR_PeripheralDST |
			  DMA_CCR4_TCIE
			  );
  DBG_DMA_CHANNEL->CPAR = (u32)&DBG_UART->DR;
  DBG_DMA_CHANNEL->CMAR = (u32)xmit_buffer_head;
  if (xmit_buffer_head < xmit_buffer_tail) {
    DBG_DMA_CHANNEL->CNDTR = xmit_buffer_tail - xmit_buffer_head;
    dma_end = xmit_buffer_tail;    
  } else {
    DBG_DMA_CHANNEL->CNDTR =  XMIT_BUFFER_END - xmit_buffer_head;
    dma_end = xmit_buffer;
  }
#ifdef DBG_ON_USART1
  NVIC_ENABLE_INT(DMA1_Channel4_IRQChannel);
  NVIC_SET_PRIORITY(DMA1_Channel4_IRQChannel, 2);
  DBG_DMA_CHANNEL->CCR |= DMA_CCR4_EN;
#elif DBG_ON_USART3
  NVIC_ENABLE_INT(DMA1_Channel2_IRQChannel);
  NVIC_SET_PRIORITY(DMA1_Channel2_IRQChannel, 2);
  DBG_DMA_CHANNEL->CCR |= DMA_CCR2_EN;
#else
#warning "DGB_DMA is not 1 or 2"
#endif
}


void
DMA1_Channel2_handler()
{
  DBG_DMA->IFCR = DBG_DMA_CHANNEL_IFCR_CGIF;
  xmit_buffer_head = dma_end;
  if (xmit_buffer_tail == xmit_buffer_head) {
    dma_running = 0;
    return;
  }
  update_dma();
}

void
DMA1_Channel4_handler()
{
  DBG_DMA->IFCR = DBG_DMA_CHANNEL_IFCR_CGIF;
  xmit_buffer_head = dma_end;
  if (xmit_buffer_tail == xmit_buffer_head) {
    dma_running = 0;
    return;
  }
  update_dma();
}

void
DMA2_Channel4_5_handler()
{
  DBG_DMA->IFCR = DBG_DMA_CHANNEL_IFCR_CGIF;
  xmit_buffer_head = dma_end;
  if (xmit_buffer_tail == xmit_buffer_head) {
    dma_running = 0;
    return;
  }
  update_dma();
}

unsigned int
dbg_send_bytes(const unsigned char *seq, unsigned int len)
{
  /* Since each of the pointers should be read atomically
     there's no need to disable interrupts */
  unsigned char *head = xmit_buffer_head;
  unsigned char *tail = xmit_buffer_tail;
  if (tail >= head) {
    /* Free space wraps */
    unsigned int xfer_len = XMIT_BUFFER_END - tail;
    unsigned int free = DBG_XMIT_BUFFER_LEN - (tail - head) - 1;
    if (len > free) len = free;
    if (xfer_len < len) {
      memcpy(tail, seq, xfer_len);
      seq += xfer_len;
      xfer_len = len - xfer_len;
      memcpy(xmit_buffer, seq, xfer_len);
      tail = xmit_buffer + xfer_len;
    } else {
      memcpy(tail, seq, len);
      tail += len;
      if (tail == XMIT_BUFFER_END) tail = xmit_buffer;
    }
  } else {
    /* Free space continuous */
    unsigned int free = (head - tail) - 1;
    if (len > free) len = free;
    memcpy(tail, seq, len);
    tail += len;
  }
  xmit_buffer_tail = tail;
  if (!dma_running) {
    dma_running = 1;
    update_dma();
  }
  return len;
}

static unsigned char dbg_write_overrun = 0;

void
dbg_putchar(const char ch)
{
  if (dbg_write_overrun) {
    if (dbg_send_bytes((const unsigned char*)"^",1) != 1) return;
  }
  dbg_write_overrun = 0;
  if (dbg_send_bytes((const unsigned char*)&ch,1) != 1) {
    dbg_write_overrun = 1;
  }
}

void
dbg_blocking_putchar(const char ch)
{
  if (dbg_write_overrun) {
    while (dbg_send_bytes((const unsigned char*)"^", 1) != 1);
  }
  dbg_write_overrun = 0;
  while (dbg_send_bytes((const unsigned char*)&ch, 1) != 1);
}

void
dbg_drain()
{
  while(xmit_buffer_tail != xmit_buffer_head);
}

int 
dbg_getbyte(unsigned char *byte)
{
  if (DBG_UART->SR & USART_SR_RXNE) {
    *byte = DBG_UART->DR & 0xFF;
    return 1;
  }
  return 0;
}
