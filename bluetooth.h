#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "misc.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"

void _Bluetooth_Execute(void);

void _BT_RCC_Init(void);
void _BT_USART1_Init(void);
void _BT_USART2_Init(void);
void _BT_NVIC_Init(void);

#endif