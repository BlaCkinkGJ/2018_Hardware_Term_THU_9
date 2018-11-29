#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "lcd.h"
#include "Touch.h"

GPIO_InitTypeDef GPIO_InitStructure;

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOB,&GPIO_InitStructure);

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

TIM_TimeBaseStructure.TIM_Prescaler = (unit16_t)(SystemCoreClock/1000000)-1;
TIM_TimeBaseStructure.TIM_Period = 20000-1;
TIM_TimeBaseStructure.TIM_ClockDivision = 0;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWN1;
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
TIM_OCInitStructure.TIM_Pulse = 1500;
TIM_OC3Init(TIM4,&TIM_OCInitStructure);
TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Disable);

TIM_ARRPreloadConfig(TIM4,ENABLE);
TIM_Cmd(TIM4,ENABLE);

void Change_PWM_Cycle(int percentx10)
{
    int pwm_pulse;
    pwm_pulse = percent * 20000 / 100;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = pwm = pulse;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
}

int main()
{
    Change_PWM_Cycle(35);
}

