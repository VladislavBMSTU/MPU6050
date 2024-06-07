#ifndef TIME_BASE_H_
#define TIME_BASE_H_

#include "stdint.h"

void Ticks_Init(uint32_t freq);
uint32_t get_Ticks();
void delay(uint32_t delay_ms);




#endif /* TIME_BASE_H_ */
