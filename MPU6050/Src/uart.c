#include "uart.h"

#include "stm32f4xx.h"
#define GPIOAEN			(1U<<0)
#define UART2EN			(1U<<17)

#define CR1_TE			(1U<<3)
#define CR1_RE			(1U<<2)

#define CR1_UE			(1U<<13)
#define SR_TXE			(1U<<7)
#define CR1_RXNEIE		(1U<<5)


#define	DMA1EN						(1U<<21)
#define CHSEL4						(1U<<27)
#define DMA_MEM_INC					(1U<<10)
#define DMA_DIR_MEM_TO_PERIPH		(1U<<6)
#define DMA_CR_TCIE				    (1U<<4)
#define DMA_CR_EN				     (1U<<0)
#define UART_CR3_DMAT				 (1U<<7)

#define SYS_FREQ		16000000
#define APB1_CLK		SYS_FREQ

#define UART_BAUDRATE		115200

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}


static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk,  uint32_t BaudRate)
{
	USARTx->BRR =  compute_uart_bd(PeriphClk,BaudRate);
}



void uart2_rxtx_init(void)
{
	/****************Configure uart gpio pin***************/
	/*Enable clock access to gpioa */
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA2 mode to alternate function mode*/
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	/*Set PA2 alternate function type to UART_TX (AF07)*/
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);


	/*Set PA3 mode to alternate function mode*/
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |= (1U<<7);

	/*Set PA3 alternate function type to UART_RX (AF07)*/
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);




	/****************Configure uart module ***************/
	/*Enable clock access to uart2 */
	RCC->APB1ENR |= UART2EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);

	/*Configure the transfer direction*/
	 USART2->CR1 =  (CR1_TE | CR1_RE);

	/*Enable uart module*/
	 USART2->CR1 |= CR1_UE;


}


void uart2_write(int ch)
{
  /*Make sure the transmit data register is empty*/
	while(!(USART2->SR & SR_TXE)){}

  /*Write to transmit data register*/
	USART2->DR	=  (ch & 0xFF);
}



int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}


