#include <stdint.h>
#include <stdio.h>
#include <gpio.h>
#include <uspi.h>
#include <stm32f10x_nvic.h>
#include <rf230hal.h>

hal_irq_callback_t hal_irq_callback = 0;

/* Which bus is the radio connected to, 1 or 2. 
   Bus 2 is the one closest to the CPU. */
#define RF230_BUS 1

#if (RF230_BUS == 1)
#define RF230_SPI_BUS SPI_BUS_1
#define RF230_SPI_BASE SPI1_BASE
#define RADIO_IRQ_PIN 11 /* PORT C */
#else
#error "Don't use this bus"
#define RF230_SPI_BUS SPI_BUS_2
#define RF230_SPI_BASE SPI2_BASE
#define RADIO_IRQ_PIN 9 /* PORT C */
#endif

static inline void spi_radio_cs_high()
{
#if (RF230_BUS == 1)
  GPIOA->BSRR = 1<<4;
#else
  GPIOB->BSRR = 1<<12;
#endif
}

static inline void spi_radio_cs_low()
{
#if (RF230_BUS == 1)
  GPIOA->BRR = 1<<4;
#else
  GPIOB->BRR = 1<<12;
#endif
}

inline void hal_set_slptr_low()
{
#if (RF230_BUS == 1)
  GPIOA->BRR = 1<<10;
#else
  GPIOB->BRR = 1<<11;
#endif
}

inline void hal_set_slptr_high()
{
#if (RF230_BUS == 1)
  GPIOA->BSRR = 1<<10;
#else
  GPIOB->BSRR = 1<<11;
#endif
}

inline uint8_t hal_get_slptr()
{
#if (RF230_BUS == 1)
  return GPIOA->ODR & 1<<10;
#else
  return GPIOB->ODR & 1<<11;
#endif
}

inline void hal_set_rst_low()
{
#if (RF230_BUS == 1)
  GPIOA->BRR = 1<<8;
#else
  GPIOB->BRR = 1<<10;
#endif
}

inline void hal_set_rst_high()
{
#if (RF230_BUS == 1)
  GPIOA->BSRR = 1<<8;
#else
  GPIOB->BSRR = 1<<10;
#endif
}

void hal_init(void)
{
  int i;
  volatile uint32_t dummy;
  printf("rf230hal init\n");
  /* Configure I/O */

#if (RF230_BUS == 1)
  printf("rf230hal: using SPI bus 1\n");

  /* printf("Initiating GPIO for radio\n"); */
  /* CS */
  GPIO_CONF_OUTPUT_PORT(A, 4, PUSH_PULL, 50);
  /* CLOCK */
  GPIO_CONF_OUTPUT_PORT(A, 5, ALT_PUSH_PULL, 50);
  /* MISO */
  GPIO_CONF_OUTPUT_PORT(A, 6, ALT_PUSH_PULL, 50);
  /* MOSI */
  GPIO_CONF_OUTPUT_PORT(A, 7, ALT_PUSH_PULL, 50);
  /* Reset */
  GPIO_CONF_OUTPUT_PORT(A, 8, PUSH_PULL, 50);
  /* Sleep */
  GPIO_CONF_OUTPUT_PORT(A, 10, PUSH_PULL, 50);

  /* Set up interrupt pin from radio */

  printf("rf230hal: init setting up interrupts\n");
  /* PA11 -> EXTI11, PA -> set EXTICR to 0 */

  AFIO->EXTICR[2] |= 0x00002000;

  /* Enable clock to peripheral IOPA */
  RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN |
		   RCC_APB2ENR_IOPCEN);

  /* Configure IRQ pin as input */
  GPIO_CONF_INPUT_PORT(C, 11, FLOATING);
  /* Unmask interrupt for interrupt line */
  EXTI->IMR |= (1 << RADIO_IRQ_PIN);

  /* Unmask event for interrupt line */ 
  EXTI->EMR |= (1 << RADIO_IRQ_PIN);
  /* Rising edge trigger  */
  EXTI->RTSR |= (1 << RADIO_IRQ_PIN);

  /* NOT falling edge trigger  */
  /* EXTI->FTSR |= (1 << RADIO_IRQ_PIN); */
  printf("Enabling interrupts\n");

  dummy = EXTI->PR;

  //NVIC->ISER[1] |= (1 << (EXTI15_10_IRQChannel & 0x1F));
  NVIC_ENABLE_INT(EXTI15_10_IRQChannel);
  NVIC_SET_PRIORITY(EXTI15_10_IRQChannel, 4);
#else
  printf("rf230hal: using SPI bus 2\n");
  /* CS */
  GPIO_CONF_OUTPUT_PORT(B, 12, PUSH_PULL, 50);
  /* CLOCK */
  GPIO_CONF_OUTPUT_PORT(B, 13, ALT_PUSH_PULL, 50);
  /* MISO */
  GPIO_CONF_OUTPUT_PORT(B, 14, ALT_PUSH_PULL, 50);
  /* MOSI */
  GPIO_CONF_OUTPUT_PORT(B, 15, ALT_PUSH_PULL, 50);
  /* Reset */
  GPIO_CONF_OUTPUT_PORT(B, 10, PUSH_PULL, 50);
  /* Sleep */
  GPIO_CONF_OUTPUT_PORT(B, 11, PUSH_PULL, 50);

  /* Set up interrupt pin from radio */

  /* PC11 -> EXTI11, PC -> set EXTICR bits to 2 */
  AFIO->EXTICR[2] &= 0xFF0F;
  AFIO->EXTICR[2] |= 0x0020;
  /* Enable clock to peripheral IOPC */
  RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN |
		   RCC_APB2ENR_IOPCEN);
  /* Configure IRQ pin as input */
  GPIO_CONF_INPUT_PORT(C, 9, FLOATING);
  /* Unmask interrupt for line 9 */ 
  EXTI->IMR |= (1 << RADIO_IRQ_PIN);
  /* Unmask event for line 9 */ 
  EXTI->EMR |= (1 << RADIO_IRQ_PIN);
  /* Rising edge trigger  */
  EXTI->RTSR |= (1 << RADIO_IRQ_PIN);
  /* Falling edge trigger  */
  /* EXTI->FTSR |= (1 << RADIO_IRQ_PIN); */
  dummy = EXTI->PR;
  NVIC->ISER[0] |= (1 << (EXTI9_5_IRQChannel & 0x1F));
#endif
  printf("rf230hal resetting\n");

  /* Assert reset */
  hal_set_rst_low();

  /* slptr */
  hal_set_slptr_low();

  /* Don't do like this...*/
  for(i=0; i<100000; i++)
    dummy = i;

  printf("rf230hal SPI\n");

  /* Init SPI */
  uspi_init(RF230_SPI_BUS, 0x05);

  printf("rf230hal init 9\n");

  /* No chip select */
  spi_radio_cs_high();

  /* De-assert reset */
  hal_set_rst_high();
  printf("rf230hal init done\n");
}



uint8_t hal_register_read(uint8_t address)
{
  uint8_t data;

  address |= 0x80;

  spi_radio_cs_low();

  (void)uspi_txrx(RF230_SPI_BUS, address);
  data = uspi_txrx(RF230_SPI_BUS, 0x0);

  spi_radio_cs_high();

  return data;
}


void hal_register_write(uint8_t address, uint8_t value)
{
    address |= 0xc0;

    spi_radio_cs_low();

    (void)uspi_txrx(RF230_SPI_BUS, address);
    (void)uspi_txrx(RF230_SPI_BUS, value);

    spi_radio_cs_high();
}


uint8_t hal_subregister_read(uint8_t address, uint8_t mask, uint8_t position)
{
    /* Read current register value and mask out subregister. */
    uint8_t register_value = hal_register_read(address);
    register_value &= mask;
    register_value >>= position; /* Align subregister value. */

    return register_value;
}

void hal_subregister_write(uint8_t address, uint8_t mask, uint8_t position,
			   uint8_t value)
{
    /* Read current register value and mask area outside the subregister. */
    volatile uint8_t register_value = hal_register_read(address);
    register_value &= ~mask;

    /* Start preparing the new subregister value. shift in place and mask. */
    value <<= position;
    value &= mask;

    value |= register_value; /* Set the new subregister value. */

    /* Write the modified register value. */
    hal_register_write(address, value);
}

void hal_frame_write(uint8_t *write_buffer, uint8_t length)
{
  uint8_t tmp;
  length &= HAL_TRX_CMD_RADDRM; /* Truncate length to maximum frame length. */
  
  spi_radio_cs_low();
  
  /* Frame write command */
  (void)uspi_txrx(RF230_SPI_BUS, HAL_TRX_CMD_FW);
  /* Length */
  (void)uspi_txrx(RF230_SPI_BUS, length);
  
  /* Download to the Frame Buffer. */
  do {
    tmp = *write_buffer++;
    (void)uspi_txrx(RF230_SPI_BUS, tmp);
    --length;
  } while (length > 0);
  
  spi_radio_cs_high();
}

void hal_frame_read(hal_rx_frame_t *rx_frame)
{
  uint8_t phy_status;
  uint8_t frame_length;
  uint8_t *rx_data;

  spi_radio_cs_low();

  phy_status = uspi_txrx(RF230_SPI_BUS, HAL_TRX_CMD_FR);
  frame_length = uspi_txrx(RF230_SPI_BUS, 0);

  /*Check for correct frame length.*/
  if ((frame_length >= HAL_MIN_FRAME_LENGTH) && 
      (frame_length <= HAL_MAX_FRAME_LENGTH)){

    rx_data = (rx_frame->data);
    rx_frame->length = frame_length;

    do {
      *rx_data++ = uspi_txrx(RF230_SPI_BUS, 0);
      
    } while (--frame_length > 0);

    rx_frame->lqi = uspi_txrx(RF230_SPI_BUS, 0);

    rx_frame->crc = 1;

  } else {
    rx_frame->length = 0;
    rx_frame->lqi    = 0;
    rx_frame->crc    = 0;
  }
  spi_radio_cs_high();

}

void delay_us(int i)
{
  for (; i > 0; i--) {          /* Needs fixing XXX */
    unsigned j;
    for (j = 250; j > 0; j--)
      asm ("nop");
  }
}

void EXTI9_5_handler()
{
  uint32_t pending;

  pending = EXTI->PR;
  /* printf("(%x) ", pending); */
  if (pending & (1 << RADIO_IRQ_PIN)) {
    EXTI->PR |= (1 << RADIO_IRQ_PIN);
    /* Call interrupt handler */
    if (hal_irq_callback) {
      (void)hal_irq_callback();
    }
      
  } else {
    /* Badness, we don't do interrupt sharing */
    /*printf("Other device is using this interrupt line\n"); */
  }
  
}

void EXTI15_10_handler()
{
  uint32_t pending;

  pending = EXTI->PR;
  /* printf("(%lx) ", pending); */
  if (pending & (1 << RADIO_IRQ_PIN)) {
    EXTI->PR |= (1 << RADIO_IRQ_PIN);
    /* Call interrupt handler */
    if (hal_irq_callback) {
      (void)hal_irq_callback();
    }
      
  } else {
    /* Badness, we don't do interrupt sharing */
    /*printf("Other device is using this interrupt line\n"); */
  }
  
}
