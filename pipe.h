#ifndef __PIPE_H__
#define __PIPE_H__

// stm32f10x system setting header
#include "stm32f10x.h"

// core setting header
#include "core_cm3.h"
#include "misc.h"

// stm32 function header
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"

// lcd header
#include "lcd.h"
#include "touch.h"

// c compiler function header
#include <stdbool.h>

// user defined header
#include "ultrasonic.h"

// initialization system
void _System_Init(void);

// initialization function declaration
void _RCC_Init(void);
void _GPIO_Init(void);
void _USART_Init(void);
void _TIM_Init(void);
void _NVIC_Init(void);
void _EXTI_Init(void);
void _ADC_Init(void);
void _ADC_Start(void);
void _DMA_Init(void);

// user defined function declaration
void delay(const int);



#endif