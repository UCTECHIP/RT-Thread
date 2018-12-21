/*********************************************************************************
*
*	PROJECT:	SDJ2102
*
*	FILENAME:	uart.c
*
*	FUNCTION:	implement the interface of uart device
*
*	AUTHOR:		liaoll
*
*	DATE:		2017/09/11
*
*	ISFINISH:	NO
*
*	STATUS:
*			1.	2017.09.13: simulation success
*			2.	2018.12.20: Successful porting on RT-Thread   --ViKing
************************************************************************************/

/* Includes --------------------------------------------------------*/
#include "platform.h"


/* Private define --------------------------------------------------*/
/* Private macro ---------------------------------------------------*/
/* Private variables -----------------------------------------------*/
/* Extern variables ------------------------------------------------*/
/* Private function prototypes -------------------------------------*/
/* Private functions -----------------------------------------------*/

/********************************************************************
* Function Name	:	uart_init
* Description	:	close all UART interrupt, set baud rate and parity mode
* Input			:	baud_rate
* Output		:	none
* Return		:	none
********************************************************************/
void uart_init(int baud_rate)
{
	int br;
	char br_l,br_h;

	// close the interrupt
	UART0_REG(UART_IE) = 0x00;
	// flush the receive fifo
//	uart_fflush();
	// set the baud rate
	br = CORE_FREQ / baud_rate;
	br_l = (char)(br % 256);
	br_h = (char)(br / 256);
	UART0_REG(UART_BPRL) = br_l;
	UART0_REG(UART_BPRH) = (UART0_REG(UART_BPRH) & 0xF0) | br_h;
	UART0_REG(UART_CON) &= ~UART_CON_ODD_EN;		// even parity
	uart_set_IE(UART_IE_FIFO_NE);
}


/********************************************************************
* Function Name	:	uart_delay
* Description	:	delay function
* Input			:	nms
* Output		:	none
* Return		:	none
********************************************************************/
void uart_delay(int nms)
{
	volatile int i;

	while(nms--)
	{
		for(i = 0; i < 12; i++)
			;
	}
}


/********************************************************************
* Function Name	:	uart_puts
* Description	:	send a string with UART
* Input			:	str
* Output		:	none
* Return		:	none
********************************************************************/
void uart_puts(char *str)
{
	volatile int i;

	UART0_REG(UART_CON) |= UART_CON_TRS;		// select transmit mode
	for(i = 0; str[i] != '\0'; i++)
	{
		UART0_REG(UART_DATA) = str[i] ;
		while(!(UART0_REG(UART_IS) & UART_IS_TXEND))
			continue;
		UART0_REG(UART_IS) &= ~UART_IS_TXEND;
		uart_delay(500);
	}
}

/********************************************************************
* Function Name	:	uart_putchar
* Description	:	send a character with UART
* Input			:	ch
* Output		:	none
* Return		:	none
********************************************************************/
void uart_putchar(char ch)
{
//	uart_fflush();
	UART0_REG(UART_CON) |= UART_CON_TRS;
	UART0_REG(UART_DATA) = ch;
	uart_delay(10);
	while(!(UART0_REG(UART_IS) & UART_IS_TXEND))
		continue;
	UART0_REG(UART_IS) &= ~UART_IS_TXEND;
	UART0_REG(UART_ECR) = (UART0_REG(UART_ECR) & 0x0F) | 0x50;//set UART_ECR with 3
	//UART0_REG(UART_ECR) = 0x30 | br_h;//equal the last line code,but the curcuit have a little bug when read the device, so use this line code
	while(!(UART0_REG(UART_IS) & UART_IS_ECNT0)); //the etu didn't arrive default value(3)
	UART0_REG(UART_IS) &= ~UART_IS_ECNT0;
}


/********************************************************************
* Function Name	:	uart_fflush
* Description	:	clear fifo
* Input			:	none
* Output		:	none
* Return		:	none
********************************************************************/
void uart_fflush(void)
{
	UART0_REG(UART_CON) |= UART_CON_FLUSH;
//	asm("nop");
//	UART0_REG(UART_CON) &= ~UART_CON_FLUSH;
}


/********************************************************************
* Function Name	:	uart_getchar
* Description	:	get a character with UART
* Input			:	none
* Output		:	a character
* Return		:	none
********************************************************************/
char uart_getchar(void)
{
	char c;
	UART0_REG(UART_CON) &= ~UART_CON_TRS;		//select receive mode
	if(UART0_REG(UART_IS) & UART_IS_TRE)
		UART0_REG(UART_IS) &= ~UART_IS_TRE;
	if(UART0_REG(UART_IS)&UART_IS_FIFO_OV)
		uart_fflush();
	while(!(UART0_REG(UART_IS) & UART_IS_FIFO_NE))
		continue;
	uart_set_IE(UART_IE_FIFO_NE);
	return UART0_REG(UART_DATA);
}


/********************************************************************
* Function Name	:	uart_set_IE
* Description	:	set UART interrupt enable register
* Input			:	ena_mask
* Output		:	none
* Return		:	none
********************************************************************/
void uart_set_IE(char ena_mask)
{
	UART0_REG(UART_IE) |= ena_mask;
}



/********************************************************************
* Function Name	:	uart_reset_IE
* Description	:	reset the interrupt enable register
* Input			:	ena_mask
* Output		:	none
* Return		:	none
********************************************************************/
void uart_reset_IE(char ena_mask)
{
	UART0_REG(UART_IE) &= ~ena_mask;
}



