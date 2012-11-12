#include <stdint.h>
#include <gpio.h>
#include <uspi.h>
#include <stm32l_nvic.h>
#include <stm32l_rcc.h>
#include "rf230hal.h"

hal_irq_callback_t hal_irq_callback = 0;

/* Which bus is the radio connected to, 1 or 2. 
   Bus 2 is the one closest to the CPU. */
#define RF230_BUS 1

#if (RF230_BUS == 1)
#define RF230_SPI_BUS SPI_BUS_1
#define RADIO_IRQ_PIN 11 /* PORT A */
#else
#error "Dont use this right now."
#define RF230_SPI_BUS SPI_PORT 2
#define RADIO_IRQ_PIN 9 /* PORT C */
#endif

static void spi_radio_cs(uint8_t hi) {
#if (RF230_BUS == 1)
  if (hi) {
    gpio_output_set(GPIOA, 4);
  } else {
    gpio_output_clear(GPIOA, 4);
  }
#else
#endif
}

inline void hal_set_slptr_low()
{
#if (RF230_BUS == 1)
  gpio_output_clear(GPIOA, 10);
#else
  gpio_output_clear(GPIOB, 11);
#endif
}

inline void hal_set_slptr_high()
{
#if (RF230_BUS == 1)
  gpio_output_set(GPIOA, 10);
#else
  gpio_output_set(GPIOB, 11);
#endif
}

inline uint8_t hal_get_slptr()
{
#if (RF230_BUS == 1)
  return gpio_output_get(GPIOA, 10);
#else
  return gpio_output_get(GPIOB, 11);
#endif
}

inline void hal_set_rst_low()
{
#if (RF230_BUS == 1)
  gpio_output_clear(GPIOA, 8);
#else
  gpio_output_clear(GPIOB, 10);
#endif
}

inline void hal_set_rst_high()
{
#if (RF230_BUS == 1)
  gpio_output_set(GPIOA, 8);
#else
  gpio_output_set(GPIOB, 10);
#endif
}

void hal_init(void)
{
  int i;
  uint32_t volatile dummy;

  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);
  RCC->APB1ENR |= (RCC_APB1ENR_SPI2EN);

  /* Configure I/O */
#if (RF230_BUS == 1)

  /* SPI: MISO; MOSI and CLK */
  gpio_conf_af(GPIOA, 5, GPIO_OUTPUT_TYPE_PPULL, GPIO_OSPEED_40MHZ, GPIO_RESISTORS_PULLDN);
  gpio_conf_af(GPIOA, 6, GPIO_OUTPUT_TYPE_PPULL, GPIO_OSPEED_40MHZ, GPIO_RESISTORS_PULLDN);
  gpio_conf_af(GPIOA, 7, GPIO_OUTPUT_TYPE_PPULL, GPIO_OSPEED_40MHZ, GPIO_RESISTORS_PULLDN);
  gpio_map_af(GPIOA, 5, GPIO_AF_SPI1);
  gpio_map_af(GPIOA, 6, GPIO_AF_SPI1);
  gpio_map_af(GPIOA, 7, GPIO_AF_SPI1);

  /* Manual CS */
  gpio_conf_output(GPIOA, 4, GPIO_OUTPUT_TYPE_PPULL, GPIO_OSPEED_40MHZ);
  gpio_set_resistors(GPIOA, 4, GPIO_RESISTORS_PULLUP);
  /* Reset */
  gpio_conf_output(GPIOA, 8, GPIO_OUTPUT_TYPE_PPULL, GPIO_OSPEED_40MHZ);
  /* Sleep */
  gpio_conf_output(GPIOA, 10, GPIO_OUTPUT_TYPE_PPULL, GPIO_OSPEED_40MHZ);

#if 0
  /* printf("Initiating GPIO for radio\r\n"); */
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

  /* PA11 -> EXTI11, PA -> set EXTICR to 0 */
  AFIO->EXTICR[2] &= 0x0FfF;

  /* Enable clock to peripheral IOPA */
  RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN |
		   RCC_APB2ENR_IOPAEN);
  /* Configure IRQ pin as input */
  GPIO_CONF_INPUT_PORT(A, 11, FLOATING);
  /* Unmask interrupt for interrupt line */ 
  EXTI->IMR |= (1 << RADIO_IRQ_PIN);
  /* Unmask event for interrupt line */ 
  EXTI->EMR |= (1 << RADIO_IRQ_PIN);
  /* Rising edge trigger  */
  EXTI->RTSR |= (1 << RADIO_IRQ_PIN);
  /* Falling edge trigger  */
  /* EXTI->FTSR |= (1 << RADIO_IRQ_PIN); */
  dummy = EXTI->PR;
  NVIC->ISER[1] |= (1 << (EXTI15_10_IRQChannel & 0x1F));
#endif
#else
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

  /* Assert reset */
  hal_set_rst_low();

  /* slptr */
  hal_set_slptr_low();

  /* Don't do like this...*/
  for(i=0; i<100000; i++)
    ;

  /* Init SPI */
  uspi_init(RF230_SPI_BUS, 000);

  /* No chip select */
  spi_radio_cs(1);

  /* De-assert reset */
  hal_set_rst_high();
}



uint8_t hal_register_read(uint8_t address)
{
  uint8_t data;

  address |= 0x80;

  spi_radio_cs(0);

  (void)uspi_txrx(RF230_SPI_BUS, address);
  data = uspi_txrx(RF230_SPI_BUS, 0x0);

  spi_radio_cs(1);

  return data;
}


void hal_register_write(uint8_t address, uint8_t value)
{
    address |= 0xc0;

    spi_radio_cs(0);

    (void)uspi_txrx(RF230_SPI_BUS, address);
    (void)uspi_txrx(RF230_SPI_BUS, value);

    spi_radio_cs(1);
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
  
  spi_radio_cs(0);
  
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
  
  spi_radio_cs(1);
}

void hal_frame_read(hal_rx_frame_t *rx_frame)
{
  uint8_t phy_status;
  uint8_t frame_length;
  uint8_t *rx_data;

  spi_radio_cs(0);

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
  spi_radio_cs(1);

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
#if 0
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
    /*printf("Other device is using this interrupt line\n\r"); */
  }
#endif
}

void EXTI15_10_handler()
{
#if 0
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
    /*printf("Other device is using this interrupt line\n\r"); */
  }
#endif
}
