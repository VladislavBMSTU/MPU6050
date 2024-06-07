#include "stm32f4xx.h"

void SystemInit (void)
{

	 SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));


}

