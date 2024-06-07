#include "time_base.h"

#include "stm32f4xx.h"


volatile uint32_t current_ticks;

void Ticks_Init(uint32_t freq)
{
	/*Load the SysTick value to be the core frequency over 1000
	 *
	 * Since th core frequency is in MHz, dividing it by 1000 will get 1ms period
	 * */
	SysTick->LOAD=(freq/1000)-1;

	/*Set the source to be internal core clock*/
	SysTick->CTRL=(1<<SysTick_CTRL_CLKSOURCE_Pos);

	/*Enable The interrupt */

	SysTick->CTRL|=(1<<SysTick_CTRL_TICKINT_Pos);

	/*Enable Systick Interrupt in NIVC*/

	NVIC_EnableIRQ(SysTick_IRQn);

	/*Enable Systick*/
	SysTick->CTRL|=(1<<SysTick_CTRL_ENABLE_Pos);


}

void SysTick_Handler()
{
	/*Increment the counter*/
	current_ticks++;
}

uint32_t get_Ticks()
{
	/*Return the counter value*/
	return current_ticks;
}

/*Spin lock the CPU to force delay*/
void delay(uint32_t delay_ms)
{

	uint32_t ticks_start=get_Ticks();

	while(get_Ticks()-ticks_start<delay_ms);
}
