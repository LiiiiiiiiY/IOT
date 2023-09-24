//-------------------------------------------------------------------
// Filename: hal_uart.c
// Description: This file contains the interface to the H/W UART driver.
//-------------------------------------------------------------------
//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------
#include "hal_defs.h"
#include "hal_board.h"
#include "hal_uart.h"

//-------------------------------------------------------------------
// MACROS
//-------------------------------------------------------------------
#define HAL_UART_ISR_RX_AVAIL() \
(uartCfg.rxTail >= uartCfg.rxHead) ? \
(uartCfg.rxTail - uartCfg.rxHead) : \
(HAL_UART_ISR_RX_MAX - uartCfg.rxHead + uartCfg.rxTail)

#define HAL_UART_ISR_TX_AVAIL() \
(uartCfg.txHead > uartCfg.txTail) ? \
(uartCfg.txHead - uartCfg.txTail - 1) : \
(HAL_UART_ISR_TX_MAX - uartCfg.txTail + uartCfg.txHead - 1)

//-------------------------------------------------------------------
// TYPEDEFS
//-------------------------------------------------------------------
// U0CSR - USART Control and Status Register.
#define CSR_MODE                   0x80
#define CSR_RE                     0x40
#define CSR_SLAVE                  0x20
#define CSR_FE                     0x10
#define CSR_ERR                    0x08
#define CSR_RX_BYTE                0x04
#define CSR_TX_BYTE                0x02
#define CSR_ACTIVE                 0x01

// U0UCR - USART UART Control Register.
#define UCR_FLUSH                  0x80
#define UCR_FLOW                   0x40
#define UCR_D9                     0x20
#define UCR_BIT9                   0x10
#define UCR_PARITY                 0x08
#define UCR_SPB                    0x04
#define UCR_STOP                   0x02
#define UCR_START                  0x01

#define UTX0IE                     0x04
#define U0RX_TX                    0x0C

#define HAL_UART_PERCFG_BIT        0x02   

#define HAL_UART_ISR_RX_MAX        128
#define HAL_UART_ISR_TX_MAX        HAL_UART_ISR_RX_MAX

typedef struct
{
    uint8 rxBuf[HAL_UART_ISR_RX_MAX];
    uint8 rxHead;
    volatile uint8 rxTail;
    uint8 rxShdw;

    uint8 txBuf[HAL_UART_ISR_TX_MAX];
    volatile uint8 txHead;
    uint8 txTail;
    uint8 txMT;
} uartCfg_t;

//-------------------------------------------------------------------
// LOCAL VARIABLES
//-------------------------------------------------------------------
uartCfg_t uartCfg;

//-------------------------------------------------------------------
// @fn      halUartInit
// @brief   Initialize the UART
// @param   none
// @return  none
//-------------------------------------------------------------------
void halUartInit1(uint32 baud)
{
    // UART Configuration
    PERCFG &= ~0x02; // Set UART0 I/O location to P0.
    P0SEL |= 0x30; // Enable Tx and Rx peripheral functions on pins.
   // ADCCFG &= ~0x30; // Make sure ADC doesnt use this.
    U1CSR |= 0xC0; // Mode is UART Mode，使能接收
   
    switch(baud)
    {
        case 1200 : 
            U1BAUD = 59; U1GCR = 5; break;
        case 2400 : 
            U1BAUD = 59; U1GCR = 6; break;
        case 4800 :
            U1BAUD = 59; U1GCR = 7; break;
        case 9600 : 
            U1BAUD = 59; U1GCR = 8; break;
        case 14400 :
            U1BAUD = 216; U1GCR = 8; break;
        case 19200 : 
            U1BAUD = 59; U1GCR = 9; break;
        case 28800 :
            U1BAUD = 216; U1GCR = 9; break;
        case 38400 : 
            U1BAUD = 59; U1GCR = 10; break;
        case 57600 :
            U1BAUD = 216; U1GCR = 10; break;
        case 76800 : 
            U1BAUD = 59; U1GCR = 11; break;
        case 115200 :
            U1BAUD = 216; U1GCR = 11; break;
        case 230400 : 
            U1BAUD = 216; U1GCR = 12; break;
        default :
            U1BAUD = 59; U1GCR = 10; break;            
    }

    // 8 bits/char; no parity; 1 stop bit; stop bit hi.
    U1UCR = UCR_STOP;
    //U1CSR |= CSR_RE;
    URX1IE = 1;
    U1DBUF = 0; // Prime the ISR pump.
    UTX1IF=0;
    uartCfg.rxHead = 0;
    uartCfg.rxTail = 0;
    uartCfg.txHead = 0;
    uartCfg.txTail = 0;
    uartCfg.rxShdw = 0;
    uartCfg.txMT = 0;

}


void halUartInit0()
{
    PERCFG&=~0x01;
    P0SEL|=0x0C;//P0_2/P0_3
    P2DIR&=~0xc0;
    U0CSR|=0x80;//设置串口为uart模式
    UTX0IF=0;
    U0BAUD = 216;
    U0GCR = 11;
}

//-------------------------------------------------------------------
// @fn      halUartRead
// @brief   Read a buffer from the UART
// @param   buf  - valid data buffer at least 'len' bytes in size
//          len  - max length number of bytes to copy to 'buf'
// @return  length of buffer that was read
//-------------------------------------------------------------------
uint16 halUartRead(uint8 *buf, uint16 len)
{
    uint16 cnt = 0;

    while ((uartCfg.rxHead != uartCfg.rxTail) && (cnt < len))
    {
        *buf++ = uartCfg.rxBuf[uartCfg.rxHead++];
        if (uartCfg.rxHead >= HAL_UART_ISR_RX_MAX)
        {
            uartCfg.rxHead = 0;
        }
        cnt++;
    }

    return cnt;
}

//-------------------------------------------------------------------
// @fn      halUartWrite
// @brief   Write a buffer to the UART.
// @param   buf - pointer to the buffer that will be written, not freed
//          len - length of
// @return  length of the buffer that was sent
//-------------------------------------------------------------------
uint16 halUartWrite(uint8 *buf, uint16 len)
{
    uint16 cnt;

    // Accept "all-or-none" on write request.
    if (HAL_UART_ISR_TX_AVAIL() < len)
    {
        return 0;
    }

    for (cnt = 0; cnt < len; cnt++)
    {
        uartCfg.txBuf[uartCfg.txTail] =  *buf++;
        uartCfg.txMT = 0;

        if (uartCfg.txTail >= HAL_UART_ISR_TX_MAX - 1)
        {
            uartCfg.txTail = 0;
        }
        else
        {
            uartCfg.txTail++;
        }

        // Keep re-enabling ISR as it might be keeping up with this loop due to other ints.
        IEN2 |= UTX0IE;
    }

    return cnt;
}

//-------------------------------------------------------------------
// @fn      halUartRxLen()
// @brief   Calculate Rx Buffer length - the number of bytes in the buffer.
// @param   none
// @return  length of current Rx Buffer
//-------------------------------------------------------------------
uint16 halUartRxLen(void)
{
    return HAL_UART_ISR_RX_AVAIL();
}

//-------------------------------------------------------------------
// @fn      halUartSuspend
// @brief   Suspend UART hardware before entering PM mode 1, 2 or 3.
// @param   None
// @return  None
//-------------------------------------------------------------------
void halUartSuspend(void)
{
    U0CSR &= ~CSR_RE;
}

//-------------------------------------------------------------------
// @fn      halUartResume
// @brief   Resume UART hardware after exiting PM mode 1, 2 or 3.
// @param   None
// @return  None
//-------------------------------------------------------------------
void halUartResume(void)
{
    U0UCR |= UCR_FLUSH;
    U0CSR |= CSR_RE;
}

//-------------------------------------------------------------------
// @fn      halUartRxIsr
// @brief   UART Receive Interrupt
// @param   None
// @return  None
//-------------------------------------------------------------------
HAL_ISR_FUNCTION(halUart0RxIsr, URX0_VECTOR)
{
    uint8 tmp = U0DBUF;
    uartCfg.rxBuf[uartCfg.rxTail] = tmp;

    // Re-sync the shadow on any 1st byte received.
    if (uartCfg.rxHead == uartCfg.rxTail)
    {
        uartCfg.rxShdw = ST0;
    }

    if (++uartCfg.rxTail >= HAL_UART_ISR_RX_MAX)
    {
        uartCfg.rxTail = 0;
    }
}

//-------------------------------------------------------------------
// @fn      halUartTxIsr
// @brief   UART Transmit Interrupt
// @param   None
// @return  None
//-------------------------------------------------------------------
HAL_ISR_FUNCTION(halUart0TxIsr, UTX0_VECTOR)
{
    if (uartCfg.txHead == uartCfg.txTail)
    {
        IEN2 &= ~UTX0IE;
        uartCfg.txMT = 1;
    }
    else
    {
        UTX0IF = 0;
        U0DBUF = uartCfg.txBuf[uartCfg.txHead++];

        if (uartCfg.txHead >= HAL_UART_ISR_TX_MAX)
        {
            uartCfg.txHead = 0;
        }
    }
}
