/***************************************************************************
*
*       PROJECT:        SDJ2102
*
*       FILENAME:       uart.h
*
*       FUNCTION:       define the interface to operate the device uart
*
*       AUTHOR:         liaoll
*
*       DATE:           2017-09-11
*
*       ISFINISH:       NO
*
*		STATUS
*				1.	2018.12.20: Successful porting on RT-Thread   --ViKing
*
***************************************************************************/

#ifndef _UART_H
#define _UART_H
/**************************************************************************
*               device register memory map
***************************************************************************/
#define UART_IS                 0x0   // uart interrupt status register
#define UART_IE                 0x1   // uart interrupt enable register
#define UART_CON                0x2   // uart control register
#define UART_DATA               0x3   // uart data register
#define UART_BPRL               0x4   // uart boud rate register low 8 bit
#define UART_BPRH               0x5   // uart boud rate register high 4 bit
#define UART_ECR                0x5   // ETU counter-value register
/**************************************************************************
*                       register bit mask
***************************************************************************/

// control register
#define UART_CON_FLUSH          1<<4  //flush the receive fifo
#define UART_CON_TRS            1<<3  //transmit/receive select 0:receive 1:transmit
#define UART_CON_RE3_EN         1<<2  //retransmission 3 times automatic enable
#define UART_CON_ODD_EN         1<<1  //ODD parity enable
#define UART_CON_DIS            1<<0  //direct inverse mode


// interrupt status register
#define UART_IS_ECNT0           1<<7  //ETU timer overflow flag
#define UART_IS_FIFO_NE         1<<6  //FIFO Not Empty flag
#define UART_IS_FIFO_HF         1<<5  //FIFO half full flag
#define UART_IS_FIFO_FU         1<<4  //FIFO full flag
#define UART_IS_FIFO_OV         1<<3  //FIFO overflow error
#define UART_IS_T2R             1<<3  //fast tramsmit to receive flag
#define UART_IS_TXEND           1<<2  //transmit finish flag
#define UART_IS_TRE             1<<1  //transmit or receive parity error flag
#define UART_IS_RETR_3          1<<0  //hardware retransmission 3 times flag

// interrupt enable register
#define UART_IE_ETOV            1<<7  //ETU timer overflow interrupt enable
#define UART_IE_FIFO_NE         1<<6  //FIFO Not Empty interrupt enable
#define UART_IE_FIFO_HF         1<<5  //FIFO Half Full interrupt enable
#define UART_IE_FIFO_FU         1<<4  //FIFO Full interrupt enable
#define UART_IE_FIFO_OV         1<<3  //FIFO Overflow interrupt enable
#define UART_IE_T2R             1<<3  //fast transmit to receive interrupt enable
#define UART_IE_TXEND           1<<2  //transmit finish interrupt enable
#define UART_IE_TRE             1<<1  //transmit/receive parity error interrupt enable
#define UART_IE_RETR_3          1<<0  //hardware retransmission 3 times interrupt enable

//frequence is 96MHz
#define CORE_FREQ                               0x5B8D800
//frequence is 48MHz
//#define CORE_FREQ				0x2DC6C00
//frequence is 24MHz
//#define CORE_FREQ				0x16E3600

/**************************************************************************
*		device operate function 
***************************************************************************/
void uart_init(int baud_rate);
void uart_puts(char * str);
char uart_getchar(void);
void uart_putchar(char ch);
void uart_fflush(void); // fifo flush
void uart_set_IE(char ena_mask); //set the interrupt enable with IE register mask
void uart_reset_IE(char ena_mask);//set the interrupt disable with IE register mask
void uart_delay(int nms);

#endif /* _UART_H */
