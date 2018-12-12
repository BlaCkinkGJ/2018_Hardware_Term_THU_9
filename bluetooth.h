#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "misc.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"

#include <stdio.h>  //sprintf
#include <string.h> //strlen
#include <stdlib.h> //atoi
#include <time.h>   //initial_현재시각

void _BT_Init(void);

void _BT_RCC_Init(void);
void _BT_GPIO_Init(void);
void _BT_USART_Init(void);
void _BT_NVIC_Init(void);

#endif