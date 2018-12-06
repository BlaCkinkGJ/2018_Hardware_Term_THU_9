#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define BEEP_OFF  GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define BEEP_ON   GPIO_SetBits(GPIOB, GPIO_Pin_5)

void _Beep_Init(void);

#endif