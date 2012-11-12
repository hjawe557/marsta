#include <sysdefs.h>
#include <lpc111x.h>
#include <stdlib.h>
#include <string.h>


void dbg_setup_uart_default(uint32_t baudrate)
{
        uint32_t fDiv;
        uint32_t regVal;
        
        NVIC_DisableIRQ(UART_IRQn);
        
        /* Set 1.6 UART RXD */
        IOCON_PIO1_6 &= ~IOCON_PIO1_6_FUNC_MASK;
        IOCON_PIO1_6 |= IOCON_PIO1_6_FUNC_UART_RXD;
        
        /* Set 1.7 UART TXD */
        IOCON_PIO1_7 &= ~IOCON_PIO1_7_FUNC_MASK;      
        IOCON_PIO1_7 |= IOCON_PIO1_7_FUNC_UART_TXD;
        
        /* Enable UART clock */
        SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_UART);
        SCB_UARTCLKDIV = SCB_UARTCLKDIV_DIV1;     /* divided by 1 */
        
        /* 8 bits, no Parity, 1 Stop bit */
        UART_U0LCR = (UART_U0LCR_Word_Length_Select_8Chars |
                      UART_U0LCR_Stop_Bit_Select_1Bits |
                      UART_U0LCR_Parity_Disabled |
                      UART_U0LCR_Parity_Select_OddParity |
                      UART_U0LCR_Break_Control_Disabled |
                      UART_U0LCR_Divisor_Latch_Access_Enabled);
        
        /* Baud rate */
        regVal = SCB_UARTCLKDIV;
        fDiv = (((CFG_CPU_CCLK * SCB_SYSAHBCLKDIV)/regVal)/16)/baudrate;
        
        UART_U0DLM = fDiv / 256;
        UART_U0DLL = fDiv % 256;
        
        /* Set DLAB back to 0 */
        UART_U0LCR = (UART_U0LCR_Word_Length_Select_8Chars |
                      UART_U0LCR_Stop_Bit_Select_1Bits |
                      UART_U0LCR_Parity_Disabled |
                      UART_U0LCR_Parity_Select_OddParity |
                      UART_U0LCR_Break_Control_Disabled |
                      UART_U0LCR_Divisor_Latch_Access_Disabled);
        
        /* Enable and reset TX and RX FIFO. */
        UART_U0FCR = (UART_U0FCR_FIFO_Enabled | 
                      UART_U0FCR_Rx_FIFO_Reset | 
                      UART_U0FCR_Tx_FIFO_Reset); 
        
        /* Read to clear the line status. */
        regVal = UART_U0LSR;
        
        /* Ensure a clean start, no data in either TX or RX FIFO. */
        while (( UART_U0LSR & (UART_U0LSR_THRE|UART_U0LSR_TEMT)) != (UART_U0LSR_THRE|UART_U0LSR_TEMT) );
        while ( UART_U0LSR & UART_U0LSR_RDR_DATA )
        {
                /* Dump data from RX FIFO */
                regVal = UART_U0RBR;
        }
        
        /* Enable the UART Interrupt */
        NVIC_EnableIRQ(UART_IRQn);
        UART_U0IER = UART_U0IER_RBR_Interrupt_Enabled | UART_U0IER_RLS_Interrupt_Enabled;
        
        return;

}

unsigned int
dbg_send_bytes(const unsigned char *seq, unsigned int len)
{
        while (len != 0) {
                while (!(UART_U0LSR & UART_U0LSR_THRE));
                UART_U0THR = *seq;
                seq++;
                len--;
        }
        
        return 0;
}

void
dbg_putchar(const char ch)
{
        while (!(UART_U0LSR & UART_U0LSR_THRE));
        UART_U0THR = ch;
}

int 
dbg_getbyte(unsigned char *byte)
{
  return 0;
}


void uartSend (uint8_t *bufferPtr, uint32_t length)
{
  while (length != 0)
  {
    /* THRE status, contain valid data */
    while ( !(UART_U0LSR & UART_U0LSR_THRE) );
    UART_U0THR = *bufferPtr;

    bufferPtr++;
    length--;
  }

  return;
}

/**************************************************************************/
/*! 
    @brief Sends a single byte over UART.

    @param[in]  byte
                Byte value to send

    @section Example

    @code 
    // Send 0xFF over UART
    uartSendByte(0xFF);
    // Send 'B' over UART (note single quotes)
    uartSendByte('B');
    @endcode

*/
/**************************************************************************/
void uartSendByte (uint8_t byte)
{
  /* THRE status, contain valid data */
  while ( !(UART_U0LSR & UART_U0LSR_THRE) );
  UART_U0THR = byte;

  return;
}
