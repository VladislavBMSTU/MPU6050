#ifndef __i2c_H
#define __i2c_H
//#include "stm32f7xx.h"                  // Device header

#include "stm32f4xx.h"                  // Device header
#include <stdint.h>
#include "stdio.h"

void i2c_init(void);

char i2c_readByte(uint8_t saddr,uint8_t maddr,uint8_t *data);

void i2c_writeByte(uint8_t saddr,uint8_t maddr,uint8_t data);

void i2c_WriteMulti(uint8_t saddr,uint8_t maddr,uint8_t *buffer, int length);

void i2c_ReadMulti(uint8_t saddr,uint8_t maddr, int n, uint8_t* data);
void i2c_bus_scan(void);
#endif

