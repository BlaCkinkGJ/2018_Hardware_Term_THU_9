#ifndef __SURVO_H__
#define __SURVO_H__

#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"


typedef struct _PWM{
    uint16_t     OCMode; // TIM_OCMode_PWM1
    uint32_t     rcc_timer;
    TIM_TypeDef* timer;
}PWM;

void delay(int); // This have to change if you merge this code

void pwm_init(PWM*);
void change_pwm_cycle(PWM*, int);

#endif
